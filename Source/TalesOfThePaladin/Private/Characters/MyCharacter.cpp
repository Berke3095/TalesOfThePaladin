#include "Characters/MyCharacter.h"

// Action library
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Camera
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

// Animation
#include "AnimInstances/MyCharacterAnimInstance.h" 
#include "Animation/AnimMontage.h"

#include "Engine/SkeletalMeshSocket.h" //Mesh socket
#include "Spells/Projectile.h" // Spells

// Kismet
#include "Kismet/KismetMathLibrary.h" // Math
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" // Sound

// Widget
#include "Widgets/SpellSwitchWidget.h" 
#include "Components/Image.h"

// Weapon
#include "Weapons/Weapon.h"

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

// Components
#include "Components/CapsuleComponent.h"

AMyCharacter::AMyCharacter() // Defaults
{
	PrimaryActorTick.bCanEverTick = true;

	// Movement attributes
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	// Check for these in editor, sometimes doesn't apply
	CapsuleComponent = GetCapsuleComponent();
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);  

	// Collision settings - Make sure custom object type is "MainCharacter"
	MeshComponent = GetMesh();
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); // custom "Projectile" object type 
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore); 

	// Camera settings
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonShoulderCamera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get mapping context:
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(FName("WeaponSocket")); //Getting socket by name   
	FTransform WeaponSocketTransform = WeaponSocket->GetSocketTransform(GetMesh());
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, WeaponSocketTransform);
		if (Weapon)
		{
			Weapon->Equip(GetMesh(), FName("WeaponSocket"));
		}
	}

	// Get reference for mycharacter animinstance 
	MyCharacterAnimInstance = Cast<UMyCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyCharacterAnimInstance)
	{
		MyCharacterAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyCharacter::OnNotifyBegin);
	}

	RightProjectileSocket = GetMesh()->GetSocketByName(FName("RightProjectileSocket"));   
	LeftProjectileSocket = GetMesh()->GetSocketByName(FName("LeftProjectileSocket")); 

	// HeavyAttackSectionIndex
	HeavyAttackSectionArray[0] = "0"; HeavyAttackSectionArray[1] = "1"; HeavyAttackSectionArray[2] = "2"; HeavyAttackSectionArray[3] = "3";  
	HeavyAttackSectionArray[4] = "4"; HeavyAttackSectionArray[5] = "5"; HeavyAttackSectionArray[6] = "6"; HeavyAttackSectionArray[7] = "7"; 
	HeavyAttackSectionArray[8] = "8";
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime); // Keeping track of delta rotations for aim offset
	UseControllerYaw(DeltaTime);
}

/*
	CHARACTER ACTIONS
*/
void AMyCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	if (IsValid(GetController()))
	{
		TurnState = ETurnState::ETS_NONE;
		// Get controller yaw rotation
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator ControlYawRotation(0, ControlRotation.Yaw, 0);

		// Get the forward and right of controller yaw rotation
		const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

		// Adding value to directions
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void AMyCharacter::Sprint(const FInputActionValue& InputValue)
{
	const bool Sprint = InputValue.Get<bool>();
	if (Sprint && MoveState != EMoveState::EMS_AimState)
	{
		// Get Dot to calculate forwardish movement
		FVector ForwardVector = GetActorForwardVector();
		FVector VelocityDirection = GetVelocity().GetSafeNormal();
		float DotProductForward = FVector::DotProduct(ForwardVector, VelocityDirection);

		if (DotProductForward > 0.9f) // If moving forward
		{
			MoveState = EMoveState::EMS_SprintState;
			ShakeOuterRadius = 200.0f;
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
		else
		{
			DropSprint();
		}
	}
}

void AMyCharacter::DropSprint()
{
	if (MoveState == EMoveState::EMS_SprintState)
	{
		MoveState = EMoveState::EMS_NONE;
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

void AMyCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	if (IsValid(GetController()))
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

void AMyCharacter::Aim(const FInputActionValue& InputValue)
{
	const bool Aim = InputValue.Get<bool>();
	if (Aim && MoveState != EMoveState::EMS_SprintState)
	{
		MoveState = EMoveState::EMS_AimState;
		GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
		Weapon->WeaponMesh->SetVisibility(false);
	}
}

void AMyCharacter::DropAim()
{
	if (MoveState == EMoveState::EMS_AimState)
	{
		MoveState = EMoveState::EMS_NONE;
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		Weapon->WeaponMesh->SetVisibility(true);
	}
}

void AMyCharacter::Attack(const FInputActionValue& InputValue)
{
	const bool Attack = InputValue.Get<bool>();
	if (Attack)
	{
		if (MoveState == EMoveState::EMS_AimState)
		{
			if (MyCharacterAnimInstance && SpellCastMontage && !MyCharacterAnimInstance->Montage_IsPlaying(SpellCastMontage)) //Check if it is not already playing  
			{
				AttackState = EAttackState::EATS_ProjectileAttacking;
				PlayAnimMontage(SpellCastMontage);
			}
		}
	}
}

void AMyCharacter::DropAttack()
{
	if (MyCharacterAnimInstance && SpellCastMontage && MyCharacterAnimInstance->Montage_IsPlaying(SpellCastMontage)) //Check if it is not already playing  
	{
		AttackState = EAttackState::EATS_NONE;
		MyCharacterAnimInstance->Montage_Stop(0.5f, SpellCastMontage);
	}
}

void AMyCharacter::HeavyAttack(const FInputActionValue& InputValue)
{
	const bool HeavyAttack = InputValue.Get<bool>();
	if (HeavyAttack && !bHeavyLocked && MoveState != EMoveState::EMS_AimState)
	{
		AttackState = EAttackState::EATS_HeavyAttacking; 
		bIsCharging = true;
		
		if (MyCharacterAnimInstance && HeavyAttackMontage && !MyCharacterAnimInstance->Montage_IsPlaying(HeavyAttackMontage)) // Play if not already playing
		{
			MyCharacterAnimInstance->Montage_Play(HeavyAttackMontage);
			MyCharacterAnimInstance->Montage_JumpToSection(HeavyAttackSectionArray[HeavyAttackIndex]);

			if (HeavyAttackIndex == 0 || HeavyAttackIndex == 3 || HeavyAttackIndex == 6) // If charging to reach loop anim, increment again
			{
				if (MyCharacterAnimInstance->Montage_IsPlaying(HeavyAttackMontage))
				{
					HeavyAttackState = EHeavyAttackState::EHAS_ChargingHeavy;
					HeavyAttackIndex++;
				}
			}
			else if (HeavyAttackIndex == 1 || HeavyAttackIndex == 4 || HeavyAttackIndex == 7) // Loop in charge anims
			{
				HeavyAttackState = EHeavyAttackState::EHAS_ChargeLooping;
				MyCharacterAnimInstance->Montage_SetNextSection(HeavyAttackSectionArray[HeavyAttackIndex], HeavyAttackSectionArray[HeavyAttackIndex], HeavyAttackMontage);
			}
		}
	}
}

void AMyCharacter::DropHeavyAttack()
{
	if (bHeavyLocked) { return; }

	bIsCharging = false;
	bHeavyLocked = true;
	if (MyCharacterAnimInstance && HeavyAttackMontage)
	{
		if (HeavyAttackState != EHeavyAttackState::EHAS_ChargeLooping) // If charge is cut half, reset 
		{
			if (MyCharacterAnimInstance->Montage_IsPlaying(HeavyAttackMontage))
			{
				MyCharacterAnimInstance->Montage_Stop(0.4f, HeavyAttackMontage);
			}
			HeavyAttackIndex = 0;
			AttackState = EAttackState::EATS_NONE;
			HeavyAttackState = EHeavyAttackState::EHAS_NONE;
			bHeavyLocked = false;
		}
		else if (MyCharacterAnimInstance->Montage_IsPlaying(HeavyAttackMontage) && HeavyAttackState == EHeavyAttackState::EHAS_ChargeLooping)
		{
			HeavyAttackState = EHeavyAttackState::EHAS_HeavyAttacking;
			HeavyAttackIndex++; // Increment to play the current attack anim
			MyCharacterAnimInstance->Montage_Play(HeavyAttackMontage);
			MyCharacterAnimInstance->Montage_JumpToSection(HeavyAttackSectionArray[HeavyAttackIndex]);
			if (HeavyAttackIndex < UE_ARRAY_COUNT(HeavyAttackSectionArray)) // Increment to charge anim
			{
				HeavyAttackIndex++;
				if (HeavyAttackIndex == UE_ARRAY_COUNT(HeavyAttackSectionArray)) // Reset if end of sequence
				{
					HeavyAttackIndex = 0;
					// AttackState set to none in anim notify begin
				}
			}
		}
	}
}

void AMyCharacter::SpecialMove(const FInputActionValue& InputValue)
{
	const bool Special = InputValue.Get<bool>();
	if (Special)
	{

	}
}

void AMyCharacter::SpellSwitchDeactive() // Release ctrl
{
	if (SpellSwitchWidget && SpellSwitchWidget->IsInViewport())
	{
		SpellSwitchWidget->RemoveFromParent();
		SpellSwitchWidget = nullptr;
		ResetSpellSwitchWidget();  
	}
}

void AMyCharacter::SpellSwitchActive(const FInputActionValue& InputValue) // Hold ctrl
{
	const bool SpellSwitchShow = InputValue.Get<bool>();
	if (SpellSwitchShow)
	{
		if (!SpellSwitchWidget && SpellSwitchWidgetClass)
		{
			SpellSwitchWidget = CreateWidget<USpellSwitchWidget>(GetWorld(), SpellSwitchWidgetClass); // Create the widget 
			if (SpellSwitchWidget)
			{
				SpellSwitchWidget->AddToViewport();
			}
		}
	}
}

void AMyCharacter::SpellKeyPressed(FKey Key) // Pick spell if pressed respectively
{
	int32 PreviousPressedKeysSize = PressedKeys.Num();
	/*FString KeyName = Key.GetDisplayName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Key pressed: %s"), *KeyName);*/

	if (SpellSwitchWidget)
	{
		const TArray<FKey> FireBallKeys = { FKey("One"), FKey("One"), FKey("Two") };
		const TArray<FKey> ThunderBallKeys = { FKey("One"), FKey("Three"), FKey("One") };

		const TArray<UImage*> FireBallNumbers = { // FireBall number images
			SpellSwitchWidget->FireBallIndex0,
			SpellSwitchWidget->FireBallIndex1,
			SpellSwitchWidget->FireBallIndex2 };
		const TArray<UImage*> ThunderBallNumbers = { // ThunderBall number images
			SpellSwitchWidget->ThunderBallIndex0,
			SpellSwitchWidget->ThunderBallIndex1,
			SpellSwitchWidget->ThunderBallIndex2 };

		if (ActiveSpellTypePick == EActiveSpellTypePick::EASTP_ProjectileTypePick)
		{
			ProjectilePick(Key, FireBallKeys, ThunderBallKeys, FireBallNumbers, ThunderBallNumbers);
		}
		else
		{
			if (Key == FireBallKeys[0] && Key == ThunderBallKeys[0]) // Projectile pick activated
			{
				ActiveSpellTypePick = EActiveSpellTypePick::EASTP_ProjectileTypePick;
				PressedKeys.Add(Key);
				IndexNum++;
				ThunderBallNumbers[0]->SetOpacity(0.5f);
				FireBallNumbers[0]->SetOpacity(0.5f);
			}
			else
			{
				ResetSpellSwitchWidget();
				PlaySpellNumberPick(0);
			}
		}
	}

	if (PressedKeys.Num() > PreviousPressedKeysSize) 
	{
		// If the current size is greater than the previous size, play the sound
		PlaySpellNumberPick(1); 
	}
}

void AMyCharacter::ProjectilePick(FKey Key,
	const TArray<FKey> FireBallKeys, const TArray<FKey> ThunderBallKeys,
	const TArray<UImage*> FireBallNumbers, const TArray<UImage*> ThunderBallNumbers)
{
	if (Key == FireBallKeys[IndexNum] && ActiveSpellPick != EActiveSpellPick::EASP_ThunderBallPick) // Start FireBall spell pick process 
	{
		ActiveSpellPick = EActiveSpellPick::EASP_FireBallPick; // Lock the pick process

		PressedKeys.Add(Key); // Add the correct key to the array

		FireBallNumbers[IndexNum]->SetOpacity(0.5f); // Lower the opacity of the image respectively
		ThunderBallNumbers[0]->SetOpacity(1.0f);

		if (IndexNum == FireBallKeys.Num() - 1) // Once reached the max index, successful
		{
			ActiveSpell = EActiveSpell::EAS_FireBallSpell;
			ChosenSkill = 0;
			SpellSwitchDeactive();
		}
		else
		{
			IndexNum++;
		}
	}
	else if (Key == ThunderBallKeys[IndexNum] && ActiveSpellPick != EActiveSpellPick::EASP_FireBallPick) // Start ThunderBall spell pick process
	{
		ActiveSpellPick = EActiveSpellPick::EASP_ThunderBallPick;

		PressedKeys.Add(Key);

		ThunderBallNumbers[IndexNum]->SetOpacity(0.5f);
		FireBallNumbers[0]->SetOpacity(1.0f);

		if (IndexNum == ThunderBallKeys.Num() - 1) // Once reached the max index, successful
		{
			ActiveSpell = EActiveSpell::EAS_ThunderBallSpell;
			ChosenSkill = 1;
			SpellSwitchDeactive();
		}
		else
		{
			IndexNum++;
		}
	}
	else
	{
		ResetSpellSwitchWidget();
		PlaySpellNumberPick(0);
	}
}

void AMyCharacter::ResetSpellSwitchWidget() // Reset widget if wrong buttons pressed
{
	PressedKeys.Empty();
	IndexNum = 0;
	ActiveSpellPick = EActiveSpellPick::EASP_NONE;
	ActiveSpellTypePick = EActiveSpellTypePick::EASTP_NONE;
	if (SpellSwitchWidget)
	{
		SpellSwitchWidget->FireBallIndex0->SetOpacity(1.0f);
		SpellSwitchWidget->FireBallIndex1->SetOpacity(1.0f);
		SpellSwitchWidget->FireBallIndex2->SetOpacity(1.0f);
		SpellSwitchWidget->ThunderBallIndex0->SetOpacity(1.0f);
		SpellSwitchWidget->ThunderBallIndex1->SetOpacity(1.0f);
		SpellSwitchWidget->ThunderBallIndex2->SetOpacity(1.0f);
	}
}

/*
	SPELLS
*/
void AMyCharacter::SpawnProjectileAtSocket(const USkeletalMeshSocket* SpawnSocket)
{
	if (SpawnSocket)
	{
		FTransform ProjectileSocketTransform = SpawnSocket->GetSocketTransform(GetMesh()); // Getting the transform of socket 
		if (ProjectileClass[ChosenSkill])
		{
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AProjectile>(ProjectileClass[ChosenSkill], ProjectileSocketTransform);
			}
		}
	}
}

void AMyCharacter::PlaySpellNumberPick(int32 SpellPickSoundNum)
{
	if (SpellPickNumberSounds[SpellPickSoundNum])
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SpellPickNumberSounds[SpellPickSoundNum]);
	}
}

/*
	MOVEMENT
*/
void AMyCharacter::AimOffset(float DeltaTime) //Setting up the offset
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size();

	if (Speed == 0.f)
	{
		// Calculating character yaw for offset
		FRotator CurrentRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, StartingRotation); // CurrentRotation - StartingRotation
		CharacterYaw = DeltaRotation.Yaw;

		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f)
	{
		//Get the yaw of camera as soon as character walks
		StartingRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		CharacterYaw = FMath::FInterpTo(CharacterYaw, 0.f, DeltaTime, 5.f);
	}

	CharacterPitch = GetBaseAimRotation().Pitch;
}

void AMyCharacter::TurnInPlace(float DeltaTime)
{
	if (FMath::Abs(CharacterYaw) > TurnInPlaceLimit)
	{
		InterptYaw = FMath::FInterpTo(InterptYaw, 0.f, DeltaTime, 5.0f);
		CharacterYaw = InterptYaw;

		if (FMath::Abs(CharacterYaw) < 5.f)
		{
			StartingRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}

		if (AttackState != EAttackState::EATS_NONE) { return; }
		if (MyCharacterAnimInstance && TurnInPlaceMontage && !MyCharacterAnimInstance->Montage_IsPlaying(TurnInPlaceMontage))
		{
			MyCharacterAnimInstance->Montage_Play(TurnInPlaceMontage);

			int32 CaseInt{};
			FName SectionName{};

			if (CharacterYaw < -TurnInPlaceLimit + 5.0f) { CaseInt = 0; TurnState = ETurnState::ETS_TurnLeft; } // Turn left
			else if (CharacterYaw > TurnInPlaceLimit - 5.0f) { CaseInt = 1; TurnState = ETurnState::ETS_TurnRight; } // Turn Right

			switch (CaseInt)
			{
			case 0:
				SectionName = FName("0");
				break;
			case 1:
				SectionName = FName("1");
				break;
			default:
				break;
			}

			MyCharacterAnimInstance->Montage_JumpToSection(SectionName, TurnInPlaceMontage);
		}
	}
	else
	{
		InterptYaw = CharacterYaw;
		TurnState = ETurnState::ETS_NONE;
	}
}

// Actor interps to camera direction /smoother than using controller rotation yaw bool
void AMyCharacter::UseControllerYaw(float DeltaTime)
{
	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime, 10.0f);
	SetActorRotation(InterpolatedRotation);
}

void AMyCharacter::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (MyCharacterAnimInstance && HeavyAttackMontage && MyCharacterAnimInstance->Montage_IsPlaying(HeavyAttackMontage)) // If the anim is coming to an end and you want to discontinue
	{
		if (NotifyName == FName("Reset") && !bIsCharging)
		{
			AttackState = EAttackState::EATS_NONE;
			HeavyAttackState = EHeavyAttackState::EHAS_NONE;
			HeavyAttackIndex = 0;
			MyCharacterAnimInstance->Montage_Stop(0.8f, HeavyAttackMontage);
		}
		if (NotifyName == FName("AttackState0"))
		{
			AttackState = EAttackState::EATS_NONE;
			HeavyAttackState = EHeavyAttackState::EHAS_NONE; 
		}
		if (NotifyName == FName("bHeavyLocked0")) // Open the lock and ready to trigger the following attack
		{
			bHeavyLocked = false;
		}
	}
}

const AWeapon* AMyCharacter::GetWeapon()
{
	if (Weapon)
	{
		return Weapon;
	}
	else
	{
		return nullptr;
	} 
}

// Bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Capturing spell select sequences
	PlayerInputComponent->BindKey(FKey("One"), IE_Pressed, this, &AMyCharacter::SpellKeyPressed);
	PlayerInputComponent->BindKey(FKey("Two"), IE_Pressed, this, &AMyCharacter::SpellKeyPressed);
	PlayerInputComponent->BindKey(FKey("Three"), IE_Pressed, this, &AMyCharacter::SpellKeyPressed);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyCharacter::Aim); 
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMyCharacter::DropAim); 
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMyCharacter::DropAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::HeavyAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &AMyCharacter::DropHeavyAttack);
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Triggered, this, &AMyCharacter::SpecialMove);
		EnhancedInputComponent->BindAction(SpellSwitchAction, ETriggerEvent::Started, this, &AMyCharacter::SpellSwitchActive);
		EnhancedInputComponent->BindAction(SpellSwitchAction, ETriggerEvent::Completed, this, &AMyCharacter::SpellSwitchDeactive);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::Sprint); 
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::DropSprint); 
	}
}





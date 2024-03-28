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
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;

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

	RightProjectileSocket = GetMesh()->GetSocketByName(FName("RightProjectileSocket"));   
	LeftProjectileSocket = GetMesh()->GetSocketByName(FName("LeftProjectileSocket")); 
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
	if (Aim)
	{
		bIsAiming = true; 
		Weapon->WeaponMesh->SetVisibility(false);
		GetCharacterMovement()->MaxWalkSpeed = 200.f; 
		
	}
}

void AMyCharacter::DropAim()
{
	bIsAiming = false;
	Weapon->WeaponMesh->SetVisibility(true);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AMyCharacter::Attack(const FInputActionValue& InputValue)
{
	const bool Attack = InputValue.Get<bool>();
	if (Attack)
	{
		if (bIsAiming)
		{
			if (MyCharacterAnimInstance && !MyCharacterAnimInstance->Montage_IsPlaying(SpellCastMontage)) //Check if it is not already playing  
			{
				PlayAnimMontage(SpellCastMontage);
			}
		}
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
	FString KeyName = Key.GetDisplayName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Key pressed: %s"), *KeyName);

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
				UE_LOG(LogTemp, Warning, TEXT("Projectile pick initiated"));
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
			UE_LOG(LogTemp, Warning, TEXT("FireBall spell picked"));
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
			UE_LOG(LogTemp, Warning, TEXT("ThunderBall spell picked"));
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
		InterptYaw = FMath::FInterpTo(InterptYaw, 0.f, DeltaTime, 10.f);
		CharacterYaw = InterptYaw;

		if (FMath::Abs(CharacterYaw) < 5.f)
		{
			StartingRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
	else
	{
		InterptYaw = CharacterYaw;
	}
}

// Actor interps to camera direction /smoother than using controller rotation yaw bool
void AMyCharacter::UseControllerYaw(float DeltaTime)
{
	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime, 10.0f);
	SetActorRotation(InterpolatedRotation);
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
		EnhancedInputComponent->BindAction(SpellSwitchAction, ETriggerEvent::Started, this, &AMyCharacter::SpellSwitchActive);
		EnhancedInputComponent->BindAction(SpellSwitchAction, ETriggerEvent::Completed, this, &AMyCharacter::SpellSwitchDeactive);
	}
}



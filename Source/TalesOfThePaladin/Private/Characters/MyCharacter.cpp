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
#include "Kismet/KismetMathLibrary.h" // Math

// Widget
#include "Widgets/SpellSwitchWidget.h" 
#include "Components/Image.h"

//Attributes
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter() // Defaults
{
	PrimaryActorTick.bCanEverTick = true;

	// Movement attributes
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;

	// Collision settings - Make sure custom object type is "MainCharacter"
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Camera settings
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonShoulderCamera"));
	Camera->SetupAttachment(SpringArm);

	// Projectile types
	ProjectileClass.SetNum(2);

	// Amount of keys we can store
	PressedKeys.SetNum(3);

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

	// Get reference for mycharacter animinstance 
	MyCharacterAnimInstance = Cast<UMyCharacterAnimInstance>(GetMesh()->GetAnimInstance());
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
		GetCharacterMovement()->MaxWalkSpeed = 200.f; 
		
	}
}

void AMyCharacter::DropAim()
{
	bIsAiming = false;
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
	FString KeyName = Key.GetDisplayName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Key pressed: %s"), *KeyName);

	if (SpellSwitchWidget)
	{
		const TArray<FKey> MeteorKeys = { FKey("One"), FKey("One"), FKey("Two") };
		const TArray<FKey> IceHammerKeys = { FKey("One"), FKey("Three"), FKey("One") };

		const TArray<UImage*> MeteorPickImages = { // Meter number images
			SpellSwitchWidget->MeteorIndex0,
			SpellSwitchWidget->MeteorIndex1,
			SpellSwitchWidget->MeteorIndex2 };
		const TArray<UImage*> IceHammerPickImages = { // IceHammer number images
			SpellSwitchWidget->IceHammerIndex0,
			SpellSwitchWidget->IceHammerIndex1,
			SpellSwitchWidget->IceHammerIndex2 };

		if (ActiveSpellTypePick == EActiveSpellTypePick::EASTP_ProjectileTypePick)
		{
			ProjectilePick(Key, MeteorKeys, IceHammerKeys, MeteorPickImages, IceHammerPickImages);
		}
		else
		{
			if (Key == MeteorKeys[0] && Key == IceHammerKeys[0]) // Projectile pick activated
			{
				ActiveSpellTypePick = EActiveSpellTypePick::EASTP_ProjectileTypePick;
				PressedKeys.Add(Key);
				IndexNum++;
				IceHammerPickImages[0]->SetOpacity(0.5f);
				MeteorPickImages[0]->SetOpacity(0.5f);
				UE_LOG(LogTemp, Warning, TEXT("Projectile pick initiated"));
			}
			else
			{
				ResetSpellSwitchWidget();
			}
		}
	}
}

void AMyCharacter::ProjectilePick(FKey Key,
	const TArray<FKey> MeteorKeys, const TArray<FKey> IceHammerKeys,
	const TArray<UImage*> MeteorPickImages, const TArray<UImage*> IceHammerPickImages)
{
	if (Key == MeteorKeys[IndexNum] && ActiveSpellPick != EActiveSpellPick::EASP_IceHammerPick) // Start meteor spell pick process
	{
		ActiveSpellPick = EActiveSpellPick::EASP_MeteorPick; // Lock the pick process

		PressedKeys.Add(Key); // Add the correct key to the array

		MeteorPickImages[IndexNum]->SetOpacity(0.5f); // Lower the opacity of the image respectively
		IceHammerPickImages[0]->SetOpacity(1.0f);

		if (IndexNum == MeteorKeys.Num() - 1) // Once reached the max index, successful
		{
			ActiveSpell = EActiveSpell::EAS_MeteorSpell;
			ChosenSkill = 0;
			SpellSwitchDeactive();
			UE_LOG(LogTemp, Warning, TEXT("Meteor spell picked"));
		}
		else
		{
			IndexNum++;
		}
	}
	else if (Key == IceHammerKeys[IndexNum] && ActiveSpellPick != EActiveSpellPick::EASP_MeteorPick) // Start icehammer spell pick process
	{
		ActiveSpellPick = EActiveSpellPick::EASP_IceHammerPick;

		PressedKeys.Add(Key);

		IceHammerPickImages[IndexNum]->SetOpacity(0.5f);
		MeteorPickImages[0]->SetOpacity(1.0f);

		if (IndexNum == IceHammerKeys.Num() - 1) // Once reached the max index, successful
		{
			ActiveSpell = EActiveSpell::EAS_IceHammerSpell;
			ChosenSkill = 1;
			SpellSwitchDeactive();
			UE_LOG(LogTemp, Warning, TEXT("IceHammer spell picked"));
		}
		else
		{
			IndexNum++;
		}
	}
	else
	{
		ResetSpellSwitchWidget();
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
		SpellSwitchWidget->MeteorIndex0->SetOpacity(1.0f);
		SpellSwitchWidget->MeteorIndex1->SetOpacity(1.0f);
		SpellSwitchWidget->MeteorIndex2->SetOpacity(1.0f);
		SpellSwitchWidget->IceHammerIndex0->SetOpacity(1.0f);
		SpellSwitchWidget->IceHammerIndex1->SetOpacity(1.0f);
		SpellSwitchWidget->IceHammerIndex2->SetOpacity(1.0f);
	}
}

/*
	SPELLS
*/
void AMyCharacter::SpawnProjectile()
{
	const USkeletalMeshSocket* ProjectileSocket = GetMesh()->GetSocketByName(FName("ProjectileSocket")); //Getting socket by name
	if (ProjectileSocket)
	{
		FTransform ProjectileSocketTransform = ProjectileSocket->GetSocketTransform(GetMesh()); // Getting the transform of socket 
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



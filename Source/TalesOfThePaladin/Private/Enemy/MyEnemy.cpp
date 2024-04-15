#include "Enemy/MyEnemy.h"

// My character
#include "Characters/MyCharacter.h"

// Components
#include "Components/CapsuleComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" // Math

// Movement
#include "AIController.h" 
#include "GameFramework/CharacterMovementComponent.h"

AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	// Check for these in editor, sometimes doesn't apply
	CapsuleComponent = GetCapsuleComponent();
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	// Collision settings - Make sure custom object type is "Enemy"
	MeshComponent = GetMesh();
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap); // Player Weapon
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Block); // Player Spell

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Smoother, rotates into the direction it walks to
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	StartingRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyEnemy::AimOffset(float DeltaTime, float& EnemyYaw, float& EnemyPitch)
{
	if (MyCharacter && MyCharacter->GetMeshComponent())
	{
		FRotator DeltaRotation;
		FVector Velocity = GetVelocity();
		float Speed = Velocity.Size();

		FVector CharacterHeadLocation = MyCharacter->GetMeshComponent()->GetBoneLocation("head"); 
		FVector EnemyHeadLocation = MeshComponent->GetBoneLocation("head"); 

		// The direction towards the player
		FVector DirectionToPlayer = (CharacterHeadLocation - EnemyHeadLocation).GetSafeNormal();
		FRotator AimRotation = DirectionToPlayer.Rotation();
		DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, StartingRotation); // AimRotation - StartingRotation

		if (Speed == 0.f)
		{
			EnemyYaw = DeltaRotation.Yaw;
		}
		else if (Speed > 0.f)
		{
			//Get the yaw of camera as soon as character walks
			StartingRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.f);
			// EnemyYaw = FMath::FInterpTo(EnemyYaw, 0.f, DeltaTime, 5.f);
		}
		EnemyYaw = DeltaRotation.Yaw;
		EnemyPitch = DeltaRotation.Pitch; 
	}
}

void AMyEnemy::ChasePlayer(float StopRadius)
{
	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && MyCharacter)
	{
		EnemyController->MoveToActor(MyCharacter, StopRadius);
	}
}

void AMyEnemy::CustomMoveTo(float DeltaTime, FVector Location, float &Speed, float Acceptance, float EnemyYaw, float MaxYaw)
{
	FVector CharacterHeadLocation = MyCharacter->GetMeshComponent()->GetBoneLocation("head");
	FVector EnemyHeadLocation = MeshComponent->GetBoneLocation("head");

	// The direction towards the player
	FVector DirectionToPlayer = (CharacterHeadLocation - EnemyHeadLocation).GetSafeNormal();
	DirectionToPlayer.Z = 0.0f; // Ignore vertical component

	FRotator AimRotation = DirectionToPlayer.Rotation();

	// Interpolate Devil's rotation towards the target rotation
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), AimRotation, DeltaTime, 5.0f);

	float Distance = FVector::Distance(Location, GetActorLocation());

	if (Distance <= Acceptance)
	{
		Speed = 0.0f;
	}
	else
	{
		Speed = 300.0f;
		SetActorRotation(InterpolatedRotation);
	}
}

void AMyEnemy::TurnInPlace(float DeltaTime, float EnemyYaw, float MaxYaw, UAnimInstance* EnemyAnimInstance, UAnimMontage* TurnInPlaceMontage, 
	EEnemyAttackState EnemyAttackState, EEnemyTurnState EnemyTurnState)
{
	if (FMath::Abs(EnemyYaw) > MaxYaw)
	{
		if (EnemyAttackState == EEnemyAttackState::EEAS_NONE && EnemyAnimInstance && TurnInPlaceMontage)
		{
			if (!EnemyAnimInstance->Montage_IsPlaying(TurnInPlaceMontage))
			{
				EnemyAnimInstance->Montage_Play(TurnInPlaceMontage);

				int32 CaseInt{};
				FName SectionName{};

				if (EnemyYaw < -MaxYaw + 5.0f) { CaseInt = 0; EnemyTurnState = EEnemyTurnState::EETS_TurnLeft; } // Turn left
				else if (EnemyYaw > MaxYaw - 5.0f) { CaseInt = 1; EnemyTurnState = EEnemyTurnState::EETS_TurnRight; } // Turn Right

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

				EnemyAnimInstance->Montage_JumpToSection(SectionName, TurnInPlaceMontage);
			}
		}
	}
}


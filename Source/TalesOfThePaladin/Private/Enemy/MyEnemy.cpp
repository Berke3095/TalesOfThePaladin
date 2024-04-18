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
#include "NavigationSystem.h"
#include "NavigationPath.h"

#include "DrawDebugHelpers.h"

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

void AMyEnemy::AimOffset(float DeltaTime1, float& EnemyYaw1, float& EnemyPitch1)
{
	if (MyCharacter && MyCharacter->GetMeshComponent())
	{
		FRotator DeltaRotation;
		FVector Velocity = GetVelocity();
		float AngularSpeed = Velocity.Size(); // Angular speed in this case

		FVector CharacterHeadLocation = MyCharacter->GetMeshComponent()->GetBoneLocation("head"); 
		FVector EnemyHeadLocation = MeshComponent->GetBoneLocation("head"); 

		// The direction towards the player
		FVector DirectionToPlayer = (CharacterHeadLocation - EnemyHeadLocation).GetSafeNormal();
		FRotator AimRotation = DirectionToPlayer.Rotation();
		DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, StartingRotation); // AimRotation - StartingRotation

		if (AngularSpeed > 0.f)
		{
			StartingRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.f); // As the enemy turns, follow with base aim rotation
		}
		EnemyYaw1 = DeltaRotation.Yaw;
		EnemyPitch1 = DeltaRotation.Pitch; 
	}
}

void AMyEnemy::ChasePlayer(float Acceptance1)
{
	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && MyCharacter)
	{
		EnemyController->MoveToActor(MyCharacter, Acceptance1);
	}
}

void AMyEnemy::CustomMoveTo(float DeltaTime1, FVector Location1, float &Speed1, float Acceptance1, float PathAcceptance1)
{
	float DistanceToLocation = FVector::Distance(Location1, GetActorLocation());
	if (DistanceToLocation > Acceptance1) { bIsAtDestination = false; }

	if (bIsAtDestination == false)
	{
		if (PathPoints.Num() == 0)
		{
			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), Location1); // Get path points
			if (NavPath)
			{
				PathIndex = 0;
				TargetLocation = Location1;
				for (FNavPathPoint Point : NavPath->PathPoints)
				{
					PathPoints.Add(Point.Location); // Add path points
				}
			}
		}
		else if (PathPoints.Num() > 0) // As the array is filled with points, execute
		{
			if (FVector::Distance(Location1, TargetLocation) > 10.0f) { PathPoints.Empty(); return; }
			// The direction towards the player
			FVector DirectionToLocation = (PathPoints[PathIndex] - GetActorLocation()).GetSafeNormal();
			DirectionToLocation.Z = 0.0f; // Ignore vertical component

			FRotator AimRotation = DirectionToLocation.Rotation();

			// Interpolate Devil's rotation towards the target rotation
			FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), AimRotation, DeltaTime1, 5.0f);

			FVector StartLocation = PathPoints[PathIndex];
			FVector EndLocation = StartLocation + FVector(0, 0, 50); // +50 Z axis
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1, 0, 5); // Change color as needed 

			float DistanceToPath = FVector::Distance(PathPoints[PathIndex], GetActorLocation());

			if (DistanceToPath <= PathAcceptance1)
			{
				PathIndex++;
				if (PathIndex == PathPoints.Num())
				{
					PathPoints.Empty(); // Reached the end of the path, clear the path points
					PathIndex = 0;
					Speed1 = 0.0f;
					bIsAtDestination = true;
					return;
				}
			}
			else if (DistanceToLocation <= Acceptance1)
			{
				PathPoints.Empty(); // Reached the end of the path, clear the path points
				PathIndex = 0;
				Speed1 = 0.0f;
				bIsAtDestination = true;
				return;
			}
			else
			{
				Speed1 = 300.0f;
				SetActorRotation(InterpolatedRotation);
			}
		}
	}
	
}

void AMyEnemy::TurnInPlace(float DeltaTime1, float& EnemyYaw1, float MaxYaw1, UAnimInstance* EnemyAnimInstance1, UAnimMontage* TurnInPlaceMontage1, 
	EEnemyAttackState EnemyAttackState1, EEnemyTurnState& EnemyTurnState1, float Speed1)
{
	if (FMath::Abs(EnemyYaw1) > MaxYaw1 && Speed1 == 0.0f)
	{
		if (EnemyAttackState == EEnemyAttackState::EEAS_NONE && EnemyAnimInstance1 && TurnInPlaceMontage1)
		{
			if (!EnemyAnimInstance1->Montage_IsPlaying(TurnInPlaceMontage1))
			{
				EnemyAnimInstance1->Montage_Play(TurnInPlaceMontage1);

				int32 CaseInt{};
				FName SectionName{};

				if (EnemyYaw1 < -MaxYaw1 + 5.0f) { CaseInt = 0; EnemyTurnState1 = EEnemyTurnState::EETS_TurnLeft; } // Turn left
				else if (EnemyYaw1 > MaxYaw1 - 5.0f) { CaseInt = 1; EnemyTurnState1 = EEnemyTurnState::EETS_TurnRight; } // Turn Right

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

				EnemyAnimInstance1->Montage_JumpToSection(SectionName, TurnInPlaceMontage1);
			}
		}
		else 
		{
			if (EnemyAnimInstance1->Montage_IsPlaying(TurnInPlaceMontage1))
			{
				EnemyAnimInstance1->Montage_Stop(0.3f, TurnInPlaceMontage1);
				EnemyTurnState1 = EEnemyTurnState::EETS_NONE;
			}
		}
	}
	else
	{
		if (!EnemyAnimInstance1->Montage_IsPlaying(TurnInPlaceMontage1)) 
		{
			EnemyTurnState1 = EEnemyTurnState::EETS_NONE; 
		}
	}
}


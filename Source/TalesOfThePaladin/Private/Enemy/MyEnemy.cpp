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
			EnemyYaw = FMath::FInterpTo(EnemyYaw, 0.f, DeltaTime, 5.f);
		}
		EnemyPitch = DeltaRotation.Pitch; 
	}
}

void AMyEnemy::ChasePlayer()
{
	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && MyCharacter)
	{
		EnemyController->MoveToActor(MyCharacter, AcceptanceRadius);
	}
}


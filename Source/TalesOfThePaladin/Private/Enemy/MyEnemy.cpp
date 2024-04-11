#include "Enemy/MyEnemy.h"

// My character
#include "Characters/MyCharacter.h"

// Components
#include "Components/CapsuleComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" // Math

#include "AIController.h" 

AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ChasePlayer();
}

void AMyEnemy::AimOffset(float DeltaTime, float EnemyYaw, float EnemyPitch)
{
	if (MyCharacter)
	{
		FVector Velocity = GetVelocity();
		float Speed = Velocity.Size();

		if (Speed == 0.f)
		{
			// The direction towards the player
			FVector DirectionToPlayer = MyCharacter->GetActorLocation() - GetActorLocation().GetSafeNormal();

			// Calculating character yaw for offset
			FRotator AimRotation = DirectionToPlayer.Rotation();
			FRotator LookRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LookRotation, StartingRotation); // CurrentRotation - StartingRotation
			EnemyYaw = DeltaRotation.Yaw;

			// TurnInPlace(DeltaTime);
		}
		if (Speed > 0.f)
		{
			//Get the yaw of camera as soon as character walks
			StartingRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			EnemyYaw = FMath::FInterpTo(EnemyYaw, 0.f, DeltaTime, 5.f);
		}

		EnemyPitch = GetBaseAimRotation().Pitch;
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


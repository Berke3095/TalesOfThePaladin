#include "Enemy/MyEnemy.h"

// My character
#include "Characters/MyCharacter.h"

// Components
#include "Components/CapsuleComponent.h"

#include "AIController.h" 

AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Check for these in editor, sometimes doesn't apply
	CapsuleComponent = GetCapsuleComponent();
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	// Collision settings - Make sure custom object type is "Enemy"
	MeshComponent = GetMesh();
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChasePlayer();
}

void AMyEnemy::ChasePlayer()
{
	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && MyCharacter)
	{
		EnemyController->MoveToActor(MyCharacter, AcceptanceRadius);
	}
}


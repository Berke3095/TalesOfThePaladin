#include "Enemy/Dog.h"

// Components
#include "Components/CapsuleComponent.h"

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

ADog::ADog()
{
	PrimaryActorTick.bCanEverTick = true;

	// Movement attributes
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	// Disable capsule for the dog for the unmatched size, created a new one in the editor
	CapsuleComponent->SetGenerateOverlapEvents(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void ADog::BeginPlay()
{
	Super::BeginPlay();
}

void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChasePlayer(AcceptanceRadius);
}

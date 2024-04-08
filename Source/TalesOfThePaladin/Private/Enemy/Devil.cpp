#include "Enemy/Devil.h"

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

ADevil::ADevil()
{
	PrimaryActorTick.bCanEverTick = true; 

	// Movement attributes
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void ADevil::BeginPlay()
{
	Super::BeginPlay();
}

void ADevil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

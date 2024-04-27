#include "Enemy/Dog.h"

// Components
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

// Reference
#include "Characters/MyCharacter.h"

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

	CustomCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CustomCapsule"));
	CustomCapsule->SetupAttachment(CapsuleComponent);
	CustomCapsule->SetGenerateOverlapEvents(true);
	CustomCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CustomCapsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CustomCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CustomCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CustomCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CustomCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap); // Player Weapon
	CustomCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Block); // Player Spell

	CustomCapsule->SetCanEverAffectNavigation(false);
}

void ADog::BeginPlay()
{
	Super::BeginPlay();
}

void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChasePlayer(AcceptanceRadius);

	if (MyCharacter)
	{
		FVector CharacterLocation{ MyCharacter->GetActorLocation() };
		float DistanceToLocation = FVector::Distance(CharacterLocation, GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToLocation);
		if (DistanceToLocation <= AcceptanceRadius + 60.f) { FaceToLocation(CharacterLocation, DeltaTime); } // For some reason dog stops at + 60 acceptance, possibly because of capsule 
	}
}

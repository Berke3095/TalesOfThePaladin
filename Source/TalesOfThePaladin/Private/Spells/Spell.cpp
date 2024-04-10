#include "Spells/Spell.h"

#include "Kismet/GameplayStatics.h"

// Components
#include "Components/BoxComponent.h"

// References
#include "Characters/MyCharacter.h"
#include "Combat/Combat.h"

ASpell::ASpell()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComponent);

	// Collision Settings - Make sure custom object type is "Spell"
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Object type "spell"
	BoxComponent->SetGenerateOverlapEvents(false);
}

void ASpell::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // MyCharacter ref
	if (MyCharacter)
	{
		CombatComponent = MyCharacter->FindComponentByClass<UCombat>(); // Combat ref 
	}
	
}

void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


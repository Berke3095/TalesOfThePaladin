#include "Weapons/PlayerWeapon.h"

// Enemy
#include "Enemy/MyEnemy.h"

// My Character
#include "Characters/MyCharacter.h"

// Components
#include "Components/BoxComponent.h"

// Timer
#include "Engine/TimerHandle.h"

APlayerWeapon::APlayerWeapon()
{
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlapBegin);
	}
}

void APlayerWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	HitEnemy = Cast<AMyEnemy>(OtherActor);
	if (HitEnemy && !HitEnemy->bIsHit)
	{
		StartHitStop();
		HitEnemy->bIsHit = true;
	}
}

void APlayerWeapon::StartHitStop()
{
	if (!MyCharacter) // Initialize if not
	{
		MyCharacter = Cast<AMyCharacter>(GetOwner());
	}
	if (MyCharacter)
	{
		MyCharacter->CustomTimeDilation = 0.0f;
		HitEnemy->CustomTimeDilation = 0.0f;
	}

	GetWorld()->GetTimerManager().SetTimer(TimeDilationTimer, this, &APlayerWeapon::StopHitStop, 0.15f, false); 
} 

void APlayerWeapon::StopHitStop()
{
	if (MyCharacter)
	{
		MyCharacter->CustomTimeDilation = 1.0f;
		HitEnemy->CustomTimeDilation = 1.0f;
	}
	GetWorldTimerManager().ClearTimer(TimeDilationTimer); 
}

void APlayerWeapon::SetbIsHit(bool BoolValue)
{
	if (HitEnemy)
	{
		HitEnemy->bIsHit = BoolValue;
	}
}

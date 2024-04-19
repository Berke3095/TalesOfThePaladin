#include "Weapons/PlayerWeapon.h"

// Enemy
#include "Enemy/MyEnemy.h"

// Components
#include "Components/BoxComponent.h"

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
	Enemy = Cast<AMyEnemy>(OtherActor);
	if (Enemy)
	{
		if (BoxComponent && BoxComponent->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit Enemy"))
	}
}

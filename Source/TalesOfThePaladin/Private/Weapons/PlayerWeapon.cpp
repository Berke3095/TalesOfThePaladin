#include "Weapons/PlayerWeapon.h"

// Components
#include "Components/BoxComponent.h"

APlayerWeapon::APlayerWeapon()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlapBegin);
}

void APlayerWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

}

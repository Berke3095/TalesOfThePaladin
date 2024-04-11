#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "DevilWeapon.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API ADevilWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:

	ADevilWeapon();

private:

	UFUNCTION() // Hammer tip overlap for hit damage
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};

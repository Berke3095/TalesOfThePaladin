#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API APlayerWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:

	APlayerWeapon();

protected:

	UFUNCTION() // Hammer tip overlap for hit damage
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};

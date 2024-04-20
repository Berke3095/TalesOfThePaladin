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

private:

	UFUNCTION() // Hammer tip overlap for hit damage
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	class AMyEnemy* HitEnemy{};
	class AMyCharacter* MyCharacter{};

	void StartHitStop();
	void StopHitStop();
	FTimerHandle TimeDilationTimer{};

public:

	FORCEINLINE void SetbIsHit(bool BoolValue);
};

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

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class AMyEnemy* HitEnemy{};
	class AMyCharacter* MyCharacter{};

	void StartHitStop();
	void StopHitStop();
	FTimerHandle TimeDilationTimer{};
};

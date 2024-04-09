#pragma once

#include "CoreMinimal.h"
#include "Spells/Spell.h"
#include "Projectile.generated.h"

class USoundCue; // Multiple sounds

UCLASS()
class TALESOFTHEPALADIN_API AProjectile : public ASpell
{
	GENERATED_BODY()
	
public:
	AProjectile(); // Defaults

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly) // Projectile move
	class UProjectileMovementComponent* ProjectileMovementComponent{};

	/*
		FUNCTIONS
	*/
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& Hit);

	/*
		ON OVERLAP EFFECTS
	*/
	UPROPERTY(EditDefaultsOnly) // Projectile explosion
	class UParticleSystem* ProjectileExplosionParticle{};

	UPROPERTY(EditDefaultsOnly) // Projectile explosion sound
	USoundCue* ProjectileExplosionSound{};

	class UAudioComponent* ProjectileAudioComponent{};

	UPROPERTY(EditDefaultsOnly) // Projectile sound
	USoundCue* ProjectileSound;
	bool bHasPlayedSound{};

	void DestroyWhenFar();
	const float DistanceToDestroy{ 7000.f };
};

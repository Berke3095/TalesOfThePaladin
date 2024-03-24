#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundCue; // Multiple sounds

UCLASS()
class TALESOFTHEPALADIN_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile(); // Defaults

private:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	/*
		REFERENCES
	*/
	class AMyCharacter* MyCharacter{};

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly) // Box collision as root
		class UBoxComponent* BoxComponent{};
	UPROPERTY(EditDefaultsOnly) // Projectile move
		class UProjectileMovementComponent* ProjectileMovementComponent{};
	// Combat class variable
	class UCombat* CombatComponent{};

	/*
		ON HIT EFFECTS
	*/
	UPROPERTY(EditDefaultsOnly) // Projectile explosion
		class UParticleSystem* ProjectileExplosion{};
	UPROPERTY(EditDefaultsOnly) // Projectile explosion sound
		USoundCue* ProjectileExplosionSound{};
	class UAudioComponent* ProjectileAudioComponent{};

	UPROPERTY(EditDefaultsOnly) // Projectile sound
		USoundCue* ProjectileSound;
	bool bHasPlayedSound{};

	/*
		FUNCTIONS
	*/
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyWhenFar();
	const float DistanceToDestroy{ 7000.f };
};
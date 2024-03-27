#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION() // Hammer tip overlap for hit damage
	void OnBoxComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	class UBoxComponent* BoxComponent{}; // For hit collision 

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMesh{}; // Weapon mesh 

	void Equip(USceneComponent* InParent, FName InSocketName);
};

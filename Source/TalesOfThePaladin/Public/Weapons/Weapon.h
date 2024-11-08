#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Attributes
#include "Attributes.h"

#include "Weapon.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API AWeapon : public AActor
{
	GENERATED_BODY()

public:

	AWeapon();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* BoxComponent{}; // For hit collision 

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMesh{}; // Weapon mesh 
public:

	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent ? BoxComponent : nullptr; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh ? WeaponMesh : nullptr; }
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner); 
};
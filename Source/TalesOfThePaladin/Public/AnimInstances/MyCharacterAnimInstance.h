#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Combat/AttackEnums.h"
#include "MyCharacterAnimInstance.generated.h"

// Character related classes
class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class TALESOFTHEPALADIN_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:

	// Animation initialization and update events
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	// Creating character reference
	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MyCharacter{};

	// Creating character movement reference
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* MyCharacterMovement{};

	// Character speed
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	// Character move direction
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterDirection{};

	/*
		OFFSETS
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterPitch{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterYaw{};

	UPROPERTY(BlueprintReadOnly)
	FTransform OffHandTransform{};

	/*
		COMBAT
	*/
	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming{};

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking{};

	UPROPERTY(BlueprintReadOnly)
	EActiveAttackCharge ActiveAttackCharge{};

	const class AWeapon* Weapon{};
};

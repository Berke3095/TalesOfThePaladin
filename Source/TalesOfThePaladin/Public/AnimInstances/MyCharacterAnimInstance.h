#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

// Enums
#include "Characters/CharacterStates.h"

#include "MyCharacterAnimInstance.generated.h"

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
	class AMyCharacter* MyCharacter{};

	// Creating character movement reference
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* MyCharacterMovement{};

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
		MOVEMENT
	*/
	UPROPERTY(BlueprintReadOnly)
	ETurnState TurnState{};

	UPROPERTY(BlueprintReadOnly)
	EMoveState MoveState{};

	/*
		COMBAT
	*/
	UPROPERTY(BlueprintReadOnly)
	EAttackState AttackState{};

	const class APlayerWeapon* PlayerWeapon{};
};

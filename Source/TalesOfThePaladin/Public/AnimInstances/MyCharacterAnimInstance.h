#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

// Character related classes
class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class TALESOFTHEPALADIN_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
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

	/*
		COMBAT
	*/
	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming{};

private:

	// Animation initialization and update events
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};

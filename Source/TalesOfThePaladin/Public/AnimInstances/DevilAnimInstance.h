// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DevilAnimInstance.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API UDevilAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

	// Animation initialization and update events
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	// Creating devil reference
	UPROPERTY(BlueprintReadOnly)
	class ADevil* Devil{};

	// Creating devil movement reference
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* DevilMovement{};

	// Character speed
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	// Character move direction
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DevilDirection{};

	/*
		OFFSETS
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DevilPitch{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DevilYaw{};
	
};

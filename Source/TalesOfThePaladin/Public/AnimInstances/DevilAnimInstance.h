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

	/*
		OFFSETS
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DevilPitch{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DevilYaw{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};
	
};

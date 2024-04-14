// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "Devil.generated.h"

class ADevilWeapon;

UCLASS()
class TALESOFTHEPALADIN_API ADevil : public AMyEnemy
{
	GENERATED_BODY()
	
public:

	ADevil();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		COMBAT
	*/
	ADevilWeapon* DevilWeapon{};
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADevilWeapon> DevilWeaponClass{};

	/*
		OFFSET
	*/
	float DevilYaw{};
	float DevilPitch{};

	/*
		MOVEMENT
	*/
	float AcceptanceRadius{ 100.f };
	float Speed{0.0f};

public:

	const float GetDevilYaw() const { return DevilYaw; }
	const float GetDevilPitch() const { return DevilPitch; }
	const float GetSpeed() const { return Speed; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "Devil.generated.h"

class ADevilWeapon;
class UAnimMontage;

UCLASS()
class TALESOFTHEPALADIN_API ADevil : public AMyEnemy
{
	GENERATED_BODY()
	
public:

	ADevil();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* TurnInPlaceMontage{};

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	class UDevilAnimInstance* DevilAnimInstance{};

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
	const float MaxYaw{30.0f};

	/*
		MOVEMENT
	*/
	const float AcceptanceRadius{ 150.f };
	const float PathAcceptanceRadius{ 140.f };
	float Speed{0.0f};
	virtual void CustomMoveTo(float DeltaTime1, FVector Location1, float& Speed1, float Acceptance1, float PathAcceptance1) override;

public:

	const float GetDevilYaw() const { return DevilYaw; }
	const float GetDevilPitch() const { return DevilPitch; }
	const float GetSpeed() const { return Speed; }
	const EEnemyTurnState GetTurnState() const { return EnemyTurnState; }
};

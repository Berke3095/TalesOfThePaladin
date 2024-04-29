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

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	class UDevilAnimInstance* DevilAnimInstance{};

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* TurnInPlaceMontage{};

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

	FORCEINLINE const float GetDevilYaw() const { return DevilYaw; }
	FORCEINLINE const float GetDevilPitch() const { return DevilPitch; }
	FORCEINLINE const float GetSpeed() const { return Speed; }
	FORCEINLINE const EEnemyTurnState GetTurnState() const { return EnemyTurnState; }
};

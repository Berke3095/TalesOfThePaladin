#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "Dog.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API ADog : public AMyEnemy
{
	GENERATED_BODY()
	
public:

	ADog();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		ANIMATION
	*/
	class UDogAnimInstance* DogAnimInstance{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	class UAnimMontage* AttackMontage{};

	/*
		MOVEMENT
	*/
	const float AcceptanceRadius{ 75.f };
	const float DefaultSpeed{ 600.0f };

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CustomCapsule{};

	/*
		COMBAT
	*/
	void Attack();
};

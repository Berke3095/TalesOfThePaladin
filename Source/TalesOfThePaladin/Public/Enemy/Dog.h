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

	void Attack();
	FTimerHandle AttackTimer{};

	/*
		MOVEMENT
	*/
	const float AcceptanceRadius{ 150.f };
	const float DefaultSpeed{ 400.0f };

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CustomCapsule{};
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	AMyEnemy();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float AcceptanceRadius{ 50.f };

	/*
		OFFSET
	*/
	void AimOffset(float DeltaTime, float& EnemyYaw, float& EnemyPitch);

private:

	/*
		COMPONENTS
	*/
	class UCapsuleComponent* CapsuleComponent{};
	class USkeletalMeshComponent* MeshComponent{};

	class AMyCharacter* MyCharacter; // Player ref

	class AAIController* EnemyController{};

	void ChasePlayer(); // Go after player

	/*
		OFFSET
	*/
	FRotator StartingRotation{}; 
};

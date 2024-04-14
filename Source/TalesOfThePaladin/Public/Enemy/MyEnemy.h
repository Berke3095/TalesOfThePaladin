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

	class AMyCharacter* MyCharacter; // Player ref

	/*
		OFFSET
	*/
	void AimOffset(float DeltaTime, float& EnemyYaw, float& EnemyPitch);

	/*
		MOVEMENT
	*/
	void ChasePlayer(float StopRadius); // Go after player
	void CustomMoveTo(float DeltaTime, FVector Location, float &Speed, float Acceptance); // For root motion movement

private:

	/*
		COMPONENTS
	*/
	class UCapsuleComponent* CapsuleComponent{};
	class USkeletalMeshComponent* MeshComponent{};

	class AAIController* EnemyController{};

	/*
		OFFSET
	*/
	FRotator StartingRotation{}; 
};

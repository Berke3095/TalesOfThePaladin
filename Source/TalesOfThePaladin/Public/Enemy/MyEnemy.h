#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyStates.h"
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
	virtual void CustomMoveTo(float DeltaTime, FVector Location, float &Speed, float Acceptance, float EnemyYaw, float MaxYaw); // For root motion movement
	void TurnInPlace(float DeltaTime, float EnemyYaw, float MaxYaw, UAnimInstance* EnemyAnimInstance, UAnimMontage* TurnInPlaceMontage, 
		EEnemyAttackState EnemyAttackState, EEnemyTurnState EnemyTurnState);

	/*
		STATES
	*/
	EEnemyTurnState EnemyTurnState = EEnemyTurnState::EETS_NONE;
	EEnemyAttackState EnemyAttackState = EEnemyAttackState::EEAS_NONE;

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

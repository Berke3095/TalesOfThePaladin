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
	void AimOffset(float DeltaTime1, float& EnemyYaw1, float& EnemyPitch1);

	/*
		MOVEMENT
	*/
	void ChasePlayer(float StopRadius1); // Go after player
	virtual void CustomMoveTo(float DeltaTime1, FVector Location1, float &Speed1, float Acceptance1); // For root motion movement
	void TurnInPlace(float DeltaTime1, float& EnemyYaw1, float MaxYaw1, UAnimInstance* EnemyAnimInstance1, UAnimMontage* TurnInPlaceMontage1, 
		EEnemyAttackState EnemyAttackState1, EEnemyTurnState& EnemyTurnState1, float Speed1);

	class UNavigationPath* NavSys{};

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

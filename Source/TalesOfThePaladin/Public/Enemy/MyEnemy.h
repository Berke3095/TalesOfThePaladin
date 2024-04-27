#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyStates.h"
#include "Attributes.h"
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
	void ChasePlayer(float Acceptance1); // Go after player
	virtual void CustomMoveTo(float DeltaTime1, FVector Location1, float &Speed1, float Acceptance1, float PathAcceptance1); // For root motion movement
	void TurnInPlace(float DeltaTime1, float& EnemyYaw1, float MaxYaw1, UAnimInstance* EnemyAnimInstance1, UAnimMontage* TurnInPlaceMontage1, 
		EEnemyAttackState EnemyAttackState1, EEnemyTurnState& EnemyTurnState1, float Speed1);
	TArray<FVector> PathPoints{};
	int32 PathIndex{};
	bool bIsAtDestination{};
	FVector TargetLocation{};
	void FaceToLocation(FVector Location1, float DeltaTime1);

	/*
		STATES
	*/
	EEnemyTurnState EnemyTurnState = EEnemyTurnState::EETS_NONE;
	EEnemyAttackState EnemyAttackState = EEnemyAttackState::EEAS_NONE;

	/*
		COMPONENTS
	*/
	class UCapsuleComponent* CapsuleComponent{};

private:

	/*
		COMPONENTS
	*/
	class USkeletalMeshComponent* MeshComponent{};

	class AAIController* EnemyController{};

	/*
		OFFSET
	*/
	FRotator StartingRotation{}; 

public:

	/*
		COMBAT
	*/
	bool bIsHit{};

	/*
		Attributes
	*/
	Attributes EnemyAttributes{};
};

#pragma once

UENUM(BlueprintType)
enum class EEnemyTurnState : uint8
{
	EETS_TurnLeft UMETA(DisplayName = "TurnLeft"),
	EETS_TurnRight UMETA(DisplayName = "TurnRight"),
	EETS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EEnemyAttackState : uint8
{
	EEAS_BasicAttacking UMETA(DisplayName = "BasicAttacking"),
	EEAS_NONE UMETA(DisplayName = "NONE")
};

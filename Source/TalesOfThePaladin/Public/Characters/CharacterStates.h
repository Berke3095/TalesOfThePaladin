#pragma once

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	EATS_HeavyAttacking UMETA(DisplayName = "HeavyAttacking"),
	EATS_ProjectileAttacking UMETA(DisplayName = "ProjectileAttacking"),
	EATS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EHeavyAttackState : uint8
{
	EHAS_ChargingHeavy UMETA(DisplayName = "ChargingHeavy"),
	EHAS_ChargeLooping UMETA(DisplayName = "ChargeLooping"),
	EHAS_HeavyAttacking UMETA(DisplayName = "HeavyAttacking"),
	EHAS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	EMS_AimState UMETA(DisplayName = "AimState"),
	EMS_SprintState UMETA(DisplayName = "SprintState"),
	EMS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	ETS_TurnLeft UMETA(DisplayName = "TurnLeft"),
	ETS_TurnRight UMETA(DisplayName = "TurnRight"),
	ETS_NONE UMETA(DisplayName = "NONE")
};

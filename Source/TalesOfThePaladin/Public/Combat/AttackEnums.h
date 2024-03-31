#pragma once

// Active attack charge enum
UENUM(BlueprintType)
enum class EActiveChargeAnim : uint8
{
	ECA_ChargingHeavyAttack1 UMETA(DisplayName = "ChargingHeavyAttack1"),
	ECA_ChargingHeavyAttack2 UMETA(DisplayName = "ChargingHeavyAttack2"),
	ECA_ChargingHeavyAttack3 UMETA(DisplayName = "ChargingHeavyAttack3"),
	ECA_NONE UMETA(DisplayName = "NONE"),
};

UENUM(BlueprintType)
enum class EActiveHeavyAttack : uint8
{
	EHA_HeavyAttack1 UMETA(DisplayName = "HeavyAttack1"),
	EHA_HeavyAttack2 UMETA(DisplayName = "HeavyAttack2"),
	EHA_HeavyAttack3 UMETA(DisplayName = "HeavyAttack3"),
	EHA_NONE UMETA(DisplayName = "NONE"),
};

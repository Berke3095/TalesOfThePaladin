#pragma once

// Active attack charge enum
UENUM(BlueprintType)
enum class EActiveAttackCharge : uint8
{
	EAC_ChargingAttack1 UMETA(DisplayName = "ChargingAttack1"),
	EAC_ChargingAttack2 UMETA(DisplayName = "ChargingAttack2"),
	EAC_ChargingAttack3 UMETA(DisplayName = "ChargingAttack3"),
	EAC_NONE UMETA(DisplayName = "NONE"),
};

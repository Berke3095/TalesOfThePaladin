#pragma once

// Active spell enum
UENUM(BlueprintType)
enum class EActiveSpell : uint8
{
	EAS_FireBallSpell UMETA(DisplayName = "FireBallSpellActive"),
	EAS_ThunderBallSpell UMETA(DisplayName = "ThunderBallSpellActive"),
	EAS_NONE UMETA(DisplayName = "NONE"),
};

/*
	SPELL PICK
*/
UENUM(BlueprintType)
enum class EActiveSpellPick : uint8
{
	EASP_FireBallPick UMETA(DisplayName = "FireBallPickActive"),
	EASP_ThunderBallPick UMETA(DisplayName = "ThunderBallPickActive"),
	EASP_NONE UMETA(DisplayName = "NONE"),
};

UENUM(BlueprintType)
enum class EActiveSpellTypePick : uint8
{
	EASTP_ProjectileTypePick UMETA(DisplayName = "ProjectileTypePickActive"),
	EASTP_NONE UMETA(DisplayName = "NONE"),
};

#pragma once

// Active spell enum
UENUM(BlueprintType)
enum class EActiveSpell : uint8
{
	EAS_MeteorSpell UMETA(DisplayName = "MeteorSpellActive"),
	EAS_IceHammerSpell UMETA(DisplayName = "IceHammerSpellActive"),
	EAS_NONE UMETA(DisplayName = "NONE"),
};

/*
	SPELL PICK
*/
UENUM(BlueprintType)
enum class EActiveSpellPick : uint8
{
	EASP_MeteorPick UMETA(DisplayName = "MeteorPickActive"),
	EASP_IceHammerPick UMETA(DisplayName = "IceHammerPickActive"),
	EASP_NONE UMETA(DisplayName = "NONE"),
};

UENUM(BlueprintType)
enum class EActiveSpellTypePick : uint8
{
	EASTP_ProjectileTypePick UMETA(DisplayName = "ProjectileTypePickActive"),
	EASTP_NONE UMETA(DisplayName = "NONE"),
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellSwitchWidget.generated.h"

// Images
class UImage;

/**
 *
 */
UCLASS()
class TALESOFTHEPALADIN_API USpellSwitchWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Images of numbers
	UPROPERTY(meta = (BindWidget))
	UImage* MeteorIndex0{};
	UPROPERTY(meta = (BindWidget))
	UImage* MeteorIndex1{};
	UPROPERTY(meta = (BindWidget))
	UImage* MeteorIndex2{};
	UPROPERTY(meta = (BindWidget))
	UImage* IceHammerIndex0{};
	UPROPERTY(meta = (BindWidget))
	UImage* IceHammerIndex1{};
	UPROPERTY(meta = (BindWidget))
	UImage* IceHammerIndex2{};

};

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
	UImage* FireBallIndex0{};
	UPROPERTY(meta = (BindWidget))
	UImage* FireBallIndex1{};
	UPROPERTY(meta = (BindWidget))
	UImage* FireBallIndex2{};
	UPROPERTY(meta = (BindWidget))
	UImage* ThunderBallIndex0{};
	UPROPERTY(meta = (BindWidget))
	UImage* ThunderBallIndex1{};
	UPROPERTY(meta = (BindWidget))
	UImage* ThunderBallIndex2{};

};

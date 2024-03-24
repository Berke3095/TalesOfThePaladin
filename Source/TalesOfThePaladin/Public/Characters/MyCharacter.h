#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" // Input action
#include "Spells/SpellEnums.h"
#include "MyCharacter.generated.h"

// Used for input mapping
class UInputAction;

// Used for widgets
class USpellSwitchWidget;

// For images
class UImage;

UCLASS()
class TALESOFTHEPALADIN_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMyCharacter(); // Defaults

protected:

	// Character anim instance reference
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	class UMyCharacterAnimInstance* MyCharacterAnimInstance{};

	/*
		INPUT FUNCTIONS
	*/
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Aim(const FInputActionValue& InputValue);
	void Attack(const FInputActionValue& InputValue);
	void SpellSwitchDeactive();
	void SpellSwitchActive(const FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* SpellCastMontage{};

	/*
		CAMERA
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USpringArmComponent* SpringArm{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCameraComponent* Camera{};

	/*
		COMBAT
	*/
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AProjectile>> ProjectileClass{};

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();

	EActiveSpellPick ActiveSpellPick = EActiveSpellPick::EASP_NONE;
	EActiveSpell ActiveSpell = EActiveSpell::EAS_NONE;
	EActiveSpellTypePick ActiveSpellTypePick = EActiveSpellTypePick::EASTP_NONE;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming{};

	/*
		OFFSET
	*/
	void AimOffset(float DeltaTime);

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // Bind functionality to input

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* CharacterMappingContext{};

	// Move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	// Look
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};

	// Charge
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AimAction{};

	// Fire
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AttackAction{};

	// Spell switch
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SpellSwitchAction{};

	/*
		WIDGETS
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<USpellSwitchWidget> SpellSwitchWidgetClass{}; // SpellSwitch widget class 
	USpellSwitchWidget* SpellSwitchWidget{}; // SpellSwitch variable

	/*
		SPELL PICK
	*/
	TArray<FKey> PressedKeys{}; // Store pressed keys
	void SpellKeyPressed(FKey Key); // Capture pressed keys

	void ProjectilePick(FKey Key,
		const TArray<FKey> MeteorKeys, const TArray<FKey> IceHammerKeys,
		const TArray<UImage*> MeteorPickImages, const TArray<UImage*> IceHammerPickImages);

	void ResetSpellSwitchWidget(); // Reset the switch widget 

	int32 IndexNum{}; // IndexNum to increment for each key pressed 
	int32 ChosenSkill{}; // Choosen skill index num

	/*
		OFFSET
	*/
	float CharacterYaw{};
	float CharacterPitch{};
	float InterptYaw{};

	FRotator StartingRotation{};
	const float TurnInPlaceLimit{ 45.f };
	void TurnInPlace(float DeltaTime);

	/*
		MOVEMENT
	*/
	void UseControllerYaw(float DeltaTime);

public:
	// to be shared with MyCharacter anim instance
	const float GetCharacterYaw() const { return CharacterYaw; }
	const float GetCharacterPitch() const { return CharacterPitch; }
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" // Input action

// Enums
#include "Spells/SpellEnums.h"
#include "Characters/CharacterStates.h"
#include "MyCharacter.generated.h"

class UInputAction; // Used for input mapping
class USpellSwitchWidget; // Used for widgets
class UImage; // For images
class USkeletalMeshSocket; // Getting skeletal mesh socket
class USoundCue; // Sounds
class AWeapon; // Class
class UAnimMontage; // Anim montages

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
	void Sprint(const FInputActionValue& InputValue);
	void DropSprint();
	void Look(const FInputActionValue& InputValue);
	void Aim(const FInputActionValue& InputValue);
	void DropAim();
	void AttackStart(const FInputActionValue& InputValue); // Used for one click increments
	void Attack(const FInputActionValue& InputValue);
	void DropAttack();
	void HeavyAttack(const FInputActionValue& InputValue);
	void DropHeavyAttack();
	void SpecialMove(const FInputActionValue& InputValue);
	void SpellSwitchDeactive();
	void SpellSwitchActive(const FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SpellCastMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HeavyAttackMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* BasicAttackMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* TurnInPlaceMontage{};

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
	TSubclassOf<class AProjectile> ProjectileClass[2];

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectileAtSocket(const USkeletalMeshSocket* SpawnSocket);

	// Projectile sockets
	UPROPERTY(BlueprintReadOnly) 
	const USkeletalMeshSocket* RightProjectileSocket {};
	UPROPERTY(BlueprintReadOnly) 
	const USkeletalMeshSocket* LeftProjectileSocket {};

	UPROPERTY(EditDefaultsOnly)
	USoundCue* SpellPickNumberSounds[2]; 

	/*
		OFFSET
	*/
	void AimOffset(float DeltaTime);

	/*
		CAMERA
	*/
	UPROPERTY(BlueprintReadOnly)
	float ShakeOuterRadius{};

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // Bind functionality to input

	/*
		STATES
	*/
	EAttackState AttackState = EAttackState::EATS_NONE;
	EMoveState MoveState = EMoveState::EMS_NONE;
	ETurnState TurnState = ETurnState::ETS_NONE;
	EHeavyAttackState HeavyAttackState = EHeavyAttackState::EHAS_NONE;

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	/*
		COMPONENTS
	*/
	class UCapsuleComponent* CapsuleComponent{}; 
	class USkeletalMeshComponent* MeshComponent{};

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* CharacterMappingContext{};

	// Move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	// Sprint
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction{};

	// Look
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};

	// Aim
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AimAction{};

	// Fire
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AttackAction{};

	// Spell switch
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SpellSwitchAction{};

	// Heavy attack
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HeavyAttackAction{};

	// Special move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SpecialAction{};

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
	const TArray<FKey> FireBallKeys, const TArray<FKey> ThunderBallKeys,
	const TArray<UImage*> FireBallNumbers, const TArray<UImage*> ThunderBallNumbers);

	void ResetSpellSwitchWidget(); // Reset the switch widget 

	int32 IndexNum{}; // IndexNum to increment for each key pressed 
	int32 ChosenSkill{}; // Choosen skill index num

	void PlaySpellNumberPick(int32 SpellPickSoundNum); 

	/*
		OFFSET
	*/
	float CharacterYaw{};
	float CharacterPitch{};
	float InterptYaw{};

	FRotator StartingRotation{};
	const float TurnInPlaceLimit{ 60.f };
	void TurnInPlace(float DeltaTime);

	/*
		MOVEMENT
	*/
	void UseControllerYaw(float DeltaTime);

	const float AimSpeed{ 200.0f };
	const float DefaultSpeed{400.0f};
	const float SprintSpeed{600.0f};

	/*
		COMBAT
	*/
	AWeapon* Weapon{};
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass{};

	// Spell pick
	EActiveSpellPick ActiveSpellPick = EActiveSpellPick::EASP_NONE;
	EActiveSpell ActiveSpell = EActiveSpell::EAS_NONE;
	EActiveSpellTypePick ActiveSpellTypePick = EActiveSpellTypePick::EASTP_NONE;

	// Heavy attack
	bool bHeavyLocked{}; // if true, not able to interrupt heavy attack anim
	bool bIsCharging{}; // If not charging again after release, stop heavy attack sequence
	int32 HeavyAttackIndex{};
	FName HeavyAttackSectionArray[9];

	// Basic attack
	int32 BasicAttackIndex{};

	/*
		CAMERA
	*/
	class UImpactCameraShake* ImpactCameraShake{};

public:
	// to be shared with MyCharacter anim instance
	const float GetCharacterYaw() const { return CharacterYaw; }
	const float GetCharacterPitch() const { return CharacterPitch; }

	// States
	const EMoveState GetMoveState() const { return MoveState; }
	const EAttackState GetAttackState() const { return AttackState; }
	const ETurnState GetTurnState() const { return TurnState; }

	// For fabric
	const AWeapon* GetWeapon();
	const USkeletalMeshComponent* GetMeshComponent();
};

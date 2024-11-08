#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" // Input action

// Enums
#include "Spells/SpellEnums.h"
#include "Characters/CharacterStates.h"

// Attributes
#include "Attributes.h"

#include "MyCharacter.generated.h"

class UInputAction; // Used for input mapping
class USpellSwitchWidget; // Used for widgets
class UImage; // For images
class USkeletalMeshSocket; // Getting skeletal mesh socket
class USoundCue; // Sounds
class APlayerWeapon; // Class
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

	// Projectile sockets
	UPROPERTY(BlueprintReadOnly) 
	const USkeletalMeshSocket* RightProjectileSocket {};
	UPROPERTY(BlueprintReadOnly) 
	const USkeletalMeshSocket* LeftProjectileSocket {};

	/*
		CAMERA
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USpringArmComponent* SpringArm{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCameraComponent* Camera{};

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
		MONTAGES
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SpellCastMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HeavyAttackMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* BasicAttackMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* TurnInPlaceMontage{};

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

	void Move(const FInputActionValue& InputValue);
	void DropMove();
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
	void AimOffset(float DeltaTime);

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

	bool bIsMoving{};

	/*
		COMBAT
	*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpell> SpellClass[2];

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectileAtSocket(const USkeletalMeshSocket* SpawnSocket);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void CollisionOn();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void CollisionOff();

	APlayerWeapon* PlayerWeapon{};
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerWeapon> PlayerWeaponClass{};

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

	class AMyEnemy* Enemy{};

	/*
		CAMERA
	*/
	class UImpactCameraShake* ImpactCameraShake{};

	/*
		SOUND
	*/
	UPROPERTY(EditDefaultsOnly)
	USoundCue* SpellPickNumberSounds[2]; 

	/*
		OTHER
	*/
	float DotProductForward{};

public:
	/*
		Attributes
	*/
	Attributes CharacterAttributes{200.0f, 100.0f, 20.0f};

	// to be shared with MyCharacter anim instance
	FORCEINLINE const float GetCharacterYaw() const { return CharacterYaw; }
	FORCEINLINE const float GetCharacterPitch() const { return CharacterPitch; }

	// States
	FORCEINLINE const EMoveState GetMoveState() const { return MoveState; }
	FORCEINLINE const EAttackState GetAttackState() const { return AttackState; }
	FORCEINLINE const ETurnState GetTurnState() const { return TurnState; }

	// For fabric
	FORCEINLINE const APlayerWeapon* GetPlayerWeapon() const { return PlayerWeapon ? PlayerWeapon : nullptr; }
	FORCEINLINE const USkeletalMeshComponent* GetMeshComponent() const { return MeshComponent ? MeshComponent : nullptr; }

	// Other
	FORCEINLINE const float GetDotProductForward() const { return DotProductForward; }
};

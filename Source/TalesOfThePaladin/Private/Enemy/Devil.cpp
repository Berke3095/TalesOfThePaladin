#include "Enemy/Devil.h"

// Weapon
#include "Weapons/DevilWeapon.h"
#include "Engine/SkeletalMeshSocket.h" //Mesh socket

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

// My character
#include "Characters/MyCharacter.h"

// Animation
#include "AnimInstances/DevilAnimInstance.h" 
#include "Animation/AnimMontage.h"

ADevil::ADevil()
{
	PrimaryActorTick.bCanEverTick = true; 
}

void ADevil::BeginPlay()
{
	Super::BeginPlay();

	const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(FName("WeaponSocket")); //Getting socket by name   
	FTransform WeaponSocketTransform = WeaponSocket->GetSocketTransform(GetMesh());
	if (DevilWeaponClass)
	{
		DevilWeapon = GetWorld()->SpawnActor<ADevilWeapon>(DevilWeaponClass, WeaponSocketTransform);
		if (DevilWeapon)
		{
			DevilWeapon->Equip(GetMesh(), FName("WeaponSocket"));
		}
	}
	if (MyCharacter)
	{
		Speed = 300.0f;
	}
	// Get reference for mycharacter animinstance 
	DevilAnimInstance = Cast<UDevilAnimInstance>(GetMesh()->GetAnimInstance());
}

void ADevil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime, DevilYaw, DevilPitch);

	if (MyCharacter)
	{
		CustomMoveTo(DeltaTime, MyCharacter->GetActorLocation(), Speed, AcceptanceRadius, PathAcceptanceRadius);
	}

	// UE_LOG(LogTemp, Warning, TEXT("DevilYaw: %f"), DevilYaw);

	/*UE_LOG(LogTemp, Warning, TEXT("DevilYaw: %f"), DevilYaw);
	UE_LOG(LogTemp, Warning, TEXT("DevilPitch: %f"), DevilPitch);*/
}



void ADevil::CustomMoveTo(float DeltaTime1, FVector Location1, float& Speed1, float Acceptance1, float PathAcceptance1)
{
	AMyEnemy::CustomMoveTo(DeltaTime1, Location1, Speed1, Acceptance1, PathAcceptance1);

	TurnInPlace(DeltaTime1, DevilYaw, MaxYaw, DevilAnimInstance, TurnInPlaceMontage, EnemyAttackState, EnemyTurnState, Speed1);
}


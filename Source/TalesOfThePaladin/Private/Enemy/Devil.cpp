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
		CustomMoveTo(DeltaTime, MyCharacter->GetActorLocation(), Speed, AcceptanceRadius, DevilYaw, MaxYaw);
	}

	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Speed);

	/*UE_LOG(LogTemp, Warning, TEXT("DevilYaw: %f"), DevilYaw);
	UE_LOG(LogTemp, Warning, TEXT("DevilPitch: %f"), DevilPitch);*/
}



void ADevil::CustomMoveTo(float DeltaTime, FVector Location, float& Speed, float Acceptance, float EnemyYaw, float MaxYaw)
{
	AMyEnemy::CustomMoveTo(DeltaTime, Location, Speed, Acceptance, EnemyYaw, MaxYaw); 

	TurnInPlace(DeltaTime, EnemyYaw, MaxYaw, DevilAnimInstance, TurnInPlaceMontage, EnemyAttackState, EnemyTurnState);
}


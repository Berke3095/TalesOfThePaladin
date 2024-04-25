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

	EnemyAttributes.Health = 200.0f;
	EnemyAttributes.Mana = 0.0f;
	EnemyAttributes.Damage = 20.0f;
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
			DevilWeapon->Equip(GetMesh(), FName("WeaponSocket"), this);
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

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), EnemyAttributes.Health);

	//AimOffset(DeltaTime, DevilYaw, DevilPitch);

	//if (MyCharacter)
	//{
	//	FVector CharacterLocation = MyCharacter->GetActorLocation();
	//	float DistanceToPlayer = FVector::Distance(CharacterLocation, GetActorLocation());
	//	FVector DevilToPlayer = CharacterLocation - GetActorLocation();
	//	float DotProduct = FVector::DotProduct(DevilToPlayer.GetSafeNormal(), GetActorForwardVector()); // Dot of Devil

	//	if (MyCharacter->GetVelocity().Size() > 0 && DistanceToPlayer > AcceptanceRadius && MyCharacter->GetDotProductForward() < 0.5f && DotProduct > 0.6f)
	//	{
	//		FVector MovementDirection = MyCharacter->GetVelocity().GetSafeNormal();
	//		FVector OffsetLocation = CharacterLocation + MovementDirection * 1000.0f;
	//		CustomMoveTo(DeltaTime, OffsetLocation, Speed, AcceptanceRadius, PathAcceptanceRadius);
	//	}
	//	else
	//	{
	//		CustomMoveTo(DeltaTime, CharacterLocation, Speed, AcceptanceRadius, PathAcceptanceRadius);
	//	}
	//}
}



void ADevil::CustomMoveTo(float DeltaTime1, FVector Location1, float& Speed1, float Acceptance1, float PathAcceptance1)
{
	AMyEnemy::CustomMoveTo(DeltaTime1, Location1, Speed1, Acceptance1, PathAcceptance1);

	TurnInPlace(DeltaTime1, DevilYaw, MaxYaw, DevilAnimInstance, TurnInPlaceMontage, EnemyAttackState, EnemyTurnState, Speed1);
}


#include "Enemy/Devil.h"

// Weapon
#include "Weapons/DevilWeapon.h"
#include "Engine/SkeletalMeshSocket.h" //Mesh socket

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

// My character
#include "Characters/MyCharacter.h"

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
}

void ADevil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime, DevilYaw, DevilPitch);

	if (MyCharacter)
	{
		CustomMoveTo(DeltaTime, MyCharacter->GetActorLocation(), Speed, AcceptanceRadius);
	}

	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Speed);

	/*UE_LOG(LogTemp, Warning, TEXT("DevilYaw: %f"), DevilYaw);
	UE_LOG(LogTemp, Warning, TEXT("DevilPitch: %f"), DevilPitch);*/
}
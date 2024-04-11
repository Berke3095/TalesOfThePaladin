#include "Enemy/Devil.h"

// Weapon
#include "Weapons/DevilWeapon.h"
#include "Engine/SkeletalMeshSocket.h" //Mesh socket

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

ADevil::ADevil()
{
	PrimaryActorTick.bCanEverTick = false; 

	// Movement attributes
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
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
}

void ADevil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

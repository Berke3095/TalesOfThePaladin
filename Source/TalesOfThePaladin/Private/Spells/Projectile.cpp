#include "Spells/Projectile.h"

#include "Kismet/GameplayStatics.h"

// Components
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h" // Store the sound to destroy afterwards
#include "Components/BoxComponent.h"

// Projectile effects
#include "Particles/ParticleSystem.h" 
#include "Sound/SoundCue.h"

// References
#include "Characters/MyCharacter.h"
#include "Combat/Combat.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	if (BoxComponent)
	{
		BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block); // Block enemy 
		BoxComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (CombatComponent)
	{
		ProjectileMovementComponent->Velocity = CombatComponent->ProjectileDirection * ProjectileMovementComponent->MaxSpeed;
	}

	if (ProjectileSound)
	{
		ProjectileAudioComponent = UGameplayStatics::SpawnSoundAttached(ProjectileSound, BoxComponent);
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCharacter)
	{
		DestroyWhenFar();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileExplosionParticle)
	{
		FRotator RotateToProjectile = (GetActorLocation() - Hit.ImpactPoint).Rotation(); // Rotate the explosion effect towards projectile
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileExplosionParticle, Hit.ImpactPoint, RotateToProjectile); // Explosion effect at hit location
	}
	if (ProjectileExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileExplosionSound, Hit.ImpactPoint); // Sound at hit location  
	}
	if (ProjectileAudioComponent)
	{
		ProjectileAudioComponent->Stop();
		ProjectileAudioComponent->DestroyComponent();
	}
	Destroy();
}

void AProjectile::DestroyWhenFar()
{
	if (MyCharacter)
	{
		float DistanceSquared = FVector::DistSquared(GetActorLocation(), MyCharacter->GetActorLocation());
		if (DistanceSquared >= DistanceToDestroy * DistanceToDestroy)
		{
			Destroy();
		}
	}
}




#include "Spells/Projectile.h"

// Components
#include "Components/BoxComponent.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h" // Store the sound to destroy afterwards

// Projectile effects
#include "Particles/ParticleSystem.h" 
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// References
#include "Characters/MyCharacter.h"
#include "Combat/Combat.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComponent);

	// Collision Settings - Make sure custom object type is "Projectile"
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); // Hit dynamic 

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // MyCharacter ref
	if (MyCharacter)
	{
		CombatComponent = MyCharacter->FindComponentByClass<UCombat>(); // Combat ref
		if (CombatComponent)
		{
			ProjectileMovementComponent->Velocity = CombatComponent->ProjectileDirection * ProjectileMovementComponent->MaxSpeed;
		}
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

// Hit actions
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

		if (ProjectileAudioComponent->IsPendingKill())
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile Audio Component Destroyed Successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile Audio Component NOT Destroyed"));
		}

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




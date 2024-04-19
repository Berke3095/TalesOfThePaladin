#include "Combat/Combat.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // Trace hit sphere
#include "Enemy/MyEnemy.h"

UCombat::UCombat()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombat::BeginPlay()
{
	Super::BeginPlay();
}

void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult{};
	DrawTrace(HitResult);
	ProjectileDirection = (End - Start).GetSafeNormal();
}

void UCombat::DrawTrace(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize{};
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get the center of the screen
	FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	FVector ViewportCenterWorldPosition{};
	FVector ViewportCenterWorldDirection{};
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		ViewportCenter,
		ViewportCenterWorldPosition,
		ViewportCenterWorldDirection
	);

	// Project center of the screen to the world
	if (bScreenToWorld)
	{
		// Trace
		Start = ViewportCenterWorldPosition;
		End = Start + ViewportCenterWorldDirection * LineRange;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);

		// Consider hitpoint as end if it hits nothing at the end
		if (!TraceHitResult.bBlockingHit) { TraceHitResult.ImpactPoint = End; }
		else
		{
			// Draw sphere where line hits
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.0f,
				12,
				FColor::Red
			);

			MarkedEnemy = Cast<AMyEnemy>(TraceHitResult.GetActor());
			if (MarkedEnemy)
			{
				
			}
		}
	}

}


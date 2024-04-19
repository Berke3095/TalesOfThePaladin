#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TALESOFTHEPALADIN_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombat();

protected:

	const float LineRange{ 4000.0f };

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setting up the projectile route
	void DrawTrace(FHitResult& TraceHitResult);

	// Start and end of the line trace from screen to forward
	FVector Start{};
	FVector End{};

	class AMyEnemy* MarkedEnemy{};

public:

	// Projectile direction
	FVector ProjectileDirection{};

	FORCEINLINE const AMyEnemy* GetHitEnemy() const { return MarkedEnemy ? MarkedEnemy : nullptr; }
};

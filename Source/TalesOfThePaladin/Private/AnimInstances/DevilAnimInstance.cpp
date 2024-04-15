#include "AnimInstances/DevilAnimInstance.h"

// Devil
#include "Enemy/Devil.h"

void UDevilAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Devil = Cast<ADevil>(TryGetPawnOwner());
}

void UDevilAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Devil)
	{
		// Getting delta rotations for offset
		DevilYaw = Devil->GetDevilYaw();
		DevilPitch = Devil->GetDevilPitch();
		Speed = Devil->GetSpeed();
		DevilTurnState = Devil->GetTurnState();
	}
}

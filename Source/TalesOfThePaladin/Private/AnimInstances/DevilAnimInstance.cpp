#include "AnimInstances/DevilAnimInstance.h"

// Devil
#include "Enemy/Devil.h"
#include "GameFramework/CharacterMovementComponent.h" 

#include "Kismet/KismetMathLibrary.h" // Math
#include "KismetAnimationLibrary.h" // Animation

void UDevilAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Devil = Cast<ADevil>(TryGetPawnOwner());
	if (Devil)
	{
		DevilMovement = Devil->GetCharacterMovement();
	}
}

void UDevilAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (DevilMovement)
	{
		// Getting the direction of devil movement
		DevilDirection = UKismetAnimationLibrary::CalculateDirection(DevilMovement->Velocity, Devil->GetActorRotation());

		// Getting the speed of the devil
		Speed = UKismetMathLibrary::VSizeXY(DevilMovement->Velocity);
	}
}

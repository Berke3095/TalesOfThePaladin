#include "AnimInstances/DogAnimInstance.h"

// Dog Ref
#include "Enemy/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"

// Kismet
#include "Kismet/KismetMathLibrary.h" // Math
#include "KismetAnimationLibrary.h" // Animation


void UDogAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Dog = Cast<ADog>(TryGetPawnOwner());

	if (Dog)
	{
		DogMovement = Dog->GetCharacterMovement();
	}
}

void UDogAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (DogMovement)
	{
		// Getting the direction of character movement
		DogDirection = UKismetAnimationLibrary::CalculateDirection(DogMovement->Velocity, Dog->GetActorRotation());

		// Getting the speed of the character
		Speed = UKismetMathLibrary::VSizeXY(DogMovement->Velocity);
	}
}

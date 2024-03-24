#include "AnimInstances/MyCharacterAnimInstance.h"

// MyCharacter
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h" // Math
#include "KismetAnimationLibrary.h" // Animation

void UMyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner()); // Getting reference to MyCharacter 
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
}

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacterMovement)
	{
		// Getting the direction of character movement
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(MyCharacterMovement->Velocity, MyCharacter->GetActorRotation());

		// Getting the speed of the character
		Speed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
	}

	/*
		Offset
	*/
	if (MyCharacter)
	{
		// Getting delta rotations for offset
		CharacterYaw = MyCharacter->GetCharacterYaw();
		CharacterPitch = MyCharacter->GetCharacterPitch();
	}
}
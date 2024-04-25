#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DogAnimInstance.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API UDogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:

	// Animation initialization and update events
	virtual void NativeInitializeAnimation() override; 
	virtual void NativeUpdateAnimation(float DeltaTime) override; 

protected:

	// Creating devil reference
	UPROPERTY(BlueprintReadOnly)
	class ADog* Dog{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* DogMovement{};

	// Dog speed
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	// Dog move direction
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DogDirection{};
};

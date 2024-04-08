// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "Devil.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API ADevil : public AMyEnemy
{
	GENERATED_BODY()
	
public:

	ADevil();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	float DefaultSpeed{ 300.f };
};

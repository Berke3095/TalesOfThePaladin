// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UCLASS()
class TALESOFTHEPALADIN_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpell();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		REFERENCES
	*/
	class AMyCharacter* MyCharacter{};

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly) // Box collision as root
	class UBoxComponent* BoxComponent{};

	// Combat class variable
	class UCombat* CombatComponent{};
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDown/FlareForgeCharacter.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"
/**
 * 
 */
UCLASS()
class FLAREFORGE_API ACharacterBase : public AFlareForgeCharacter
{
	GENERATED_BODY()

public:
	//Set default values for this character's properties
	ACharacterBase();

protected:
	//Called when game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};

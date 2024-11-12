// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class FLAREFORGE_API AMyPlayerCharacter : public AMyCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilitySystemComponent();
	//void InitHUD() const;
};

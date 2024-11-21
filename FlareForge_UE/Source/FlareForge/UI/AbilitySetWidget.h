// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySetWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UAbilitySetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets the reference to MyCharacterBase
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterReference(AMyCharacterBase* InCharacter);

protected:
	// Reference to MyCharacterBase
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AMyCharacterBase* CharacterReference;
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "TopDown/FlareForgeCharacter.h"
#include "AbilitySystemInterface.h"
#include "MyCharacterAttributeSet.h"
#include "MyAbilitySystemComponent.h"
#include "CharacterBase.generated.h"
/**
 * 
 */
class UMyCharacterAttributeSet;
class UMyAbilitySystemComponent;

UCLASS()
class FLAREFORGE_API ACharacterBase : public AFlareForgeCharacter, public IAbilitySystemInterface
{
	
	GENERATED_BODY()

public:
	//Set default values for this character's properties
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UMyAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMyCharacterAttributeSet> AttributeSet;
	
};

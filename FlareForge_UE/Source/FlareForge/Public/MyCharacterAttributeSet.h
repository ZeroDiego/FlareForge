// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" //for attribute accessors
#include "MyCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class FLAREFORGE_API UMyCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UMyCharacterAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData MaxHealth;

	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, MaxHealth);

	
};

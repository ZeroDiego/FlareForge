// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UMyCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	
	UPROPERTY()
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Health)

	UPROPERTY()
	FGameplayAttributeData Armor = 100.0f;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Armor)

	UPROPERTY()
	FGameplayAttributeData Damage = 100.0f;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Damage)

	UPROPERTY()
	FGameplayAttributeData Knockback = 100.0f;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Knockback)

	UPROPERTY()
	FGameplayAttributeData Speed = 100.0f;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Speed)

/*
	//This is called whenever attributes are about to change
	virtual void UMyCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override
	{
		Super::PreAttributeChange(Attribute, NewValue);
		//Are we trying to change the health attribute?
		if(Attribute == GetHealthAttribute())
		{
			//Check the current health
			UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
			const float CurrentHealthValue = Health.GetBaseValue();

			// Clamp it to 0 to 100
			NewValue = FMath::Clamp<float>(NewValue, 0, 100);

			//Then override it to the clamped value if it needs to be clamped
			if(!FMath::IsNearlyEqual(CurrentHealthValue, NewValue))
			{
				AbilityComp->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Override, NewValue);
			}
		}
	}*/
};

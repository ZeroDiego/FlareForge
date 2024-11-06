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

	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Level);
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Power", ReplicatedUsing = OnRep_Power)
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Power);


	//Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turn into -Health
	//Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMyCharacterAttributeSet, Damage);

	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_Power(const FGameplayAttributeData& OldPower);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

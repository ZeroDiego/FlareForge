// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecievedDamageDelegate, UMyAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS(Blueprintable)

class FLAREFORGE_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FRecievedDamageDelegate RecievedDamage;

	virtual void RecieveDamage(UMyAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};

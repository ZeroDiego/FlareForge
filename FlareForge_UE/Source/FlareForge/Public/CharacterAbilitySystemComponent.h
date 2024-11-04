// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecievedDamageDelegate, UCharacterAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FRecievedDamageDelegate RecievedDamage;

	virtual void RecieveDamage(UCharacterAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};

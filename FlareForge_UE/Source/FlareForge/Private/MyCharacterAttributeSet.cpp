// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

UMyCharacterAttributeSet::UMyCharacterAttributeSet()
{
	// Initialize any attributes or variables here
}

void UMyCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyCharacterAttributeSet, Level, OldLevel);
}

void UMyCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
GAMEPLAYATTRIBUTE_REPNOTIFY(UMyCharacterAttributeSet, Health, OldHealth);
}

void UMyCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UMyCharacterAttributeSet::OnRep_Power(const FGameplayAttributeData& OldPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyCharacterAttributeSet, Power, OldPower);
}

void UMyCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyCharacterAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyCharacterAttributeSet, Power, COND_None, REPNOTIFY_Always);

}


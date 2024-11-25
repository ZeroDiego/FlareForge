// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"


AMyPlayerState::AMyPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<ULucasAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UMyCharacterAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMyCharacterAttributeSet* AMyPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AMyPlayerState::SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility)
{
	if (!NewAbility) return;

	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Replace existing ability at index
		SelectedAbilities[Index] = NewAbility;
	}
	else if (Index >= 0)
	{
		// Resize array if index is out of bounds and set ability
		SelectedAbilities.SetNum(Index + 1);
		SelectedAbilities[Index] = NewAbility;
	}
}

void AMyPlayerState::RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove)
{
	if (SelectedAbilities.Contains(AbilityToRemove))
	{
		SelectedAbilities.Remove(AbilityToRemove);
	}
}

void AMyPlayerState::TransferAbilitiesToASC()
{
	if (!AbilitySystemComponent) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : SelectedAbilities)
	{
		if (AbilityClass)
		{
			const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	SelectedAbilities.Empty(); // Clear after transferring
}
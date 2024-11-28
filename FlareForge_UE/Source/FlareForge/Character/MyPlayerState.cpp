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
	bReplicates = true;
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
	if (!HasAuthority() || !NewAbility)
	{
		return;
	}

	// Ensure that the index is within bounds
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Replace the existing ability at this index
		SelectedAbilities[Index] = NewAbility;
	}
	else
	{
		// Resize the array to accommodate new index
		if (Index >= 0)
		{
			SelectedAbilities.SetNum(Index + 1);
			SelectedAbilities[Index] = NewAbility;
		}
	}
}

void AMyPlayerState::RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove)
{
	// Check if AbilitySystemComponent is valid and we have authority
	if (!HasAuthority() || !AbilityToRemove)
	{
		return;
	}

	// Check if the ability exists in the SelectedAbilities array
	if (SelectedAbilities.Contains(AbilityToRemove))
	{
		// Remove the ability from the array
		SelectedAbilities.Remove(AbilityToRemove);
	}
}

void AMyPlayerState::TransferAbilitiesToASC()
{
	if (!AbilitySystemComponent) return;

	// Iterate over the first four indices: 0, 1, 2, and 3
	for (int32 Index = 0; Index < 4; ++Index)
	{
		if (SelectedAbilities.IsValidIndex(Index))
		{
			TSubclassOf<UGameplayAbility> AbilityClass = SelectedAbilities[Index];
			if (AbilityClass)
			{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);

				// Log ability assignment for debugging
				UE_LOG(LogTemp, Log, TEXT("Granted Ability: %s"), *AbilityClass->GetName());
			}
		}
	}
}

const TArray<TSubclassOf<UGameplayAbility>>& AMyPlayerState::GetSelectedAbilities() const
{
	return SelectedAbilities;
}
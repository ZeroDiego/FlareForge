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
	if (!NewAbility) return;

	if (SelectedAbilities.IsValidIndex(Index))
	{
		SelectedAbilities[Index] = NewAbility;
	}
	else if (Index >= 0)
	{
		SelectedAbilities.SetNum(Index + 1);
		SelectedAbilities[Index] = NewAbility;
	}

	// Automatically grant ability if AbilitySystemComponent exists
	if (AbilitySystemComponent && NewAbility)
	{
		const FGameplayAbilitySpec AbilitySpec(NewAbility, 1);
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(AbilitySpec);

		// Log success
		if (Handle.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully granted ability: %s"), *NewAbility->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to grant ability: %s"), *NewAbility->GetName());
		}
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

			// Log ability assignment for debugging
			UE_LOG(LogTemp, Log, TEXT("Granted Ability: %s"), *AbilityClass->GetName());
		}
	}
	SelectedAbilities.Empty();
}
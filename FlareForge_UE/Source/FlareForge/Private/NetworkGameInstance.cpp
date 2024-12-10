// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

void UNetworkGameInstance::SetGameplayAbilitySpecAtIndex_Implementation(const FGameplayAbilitySpec NewGameplayAbilitySpec, const int32 AtIndex)
{
	GameplayAbilitySpec.Insert(NewGameplayAbilitySpec, AtIndex);
}

TArray<FGameplayAbilitySpec> UNetworkGameInstance::GetGameplayAbilitySpec() const
{
	return GameplayAbilitySpec;
}

void UNetworkGameInstance::SetSelectedAbilities_Implementation(const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities)
{
	SelectedAbilities = NewSelectedAbilities;
	for (const TSubclassOf<UGameplayAbility> Ability : SelectedAbilities)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EffectHandle %s"), *Ability->GetName()));
	}
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndex(const int32 Index) const
{
	// Check if the index is valid
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Return the ability at the specified index
		return SelectedAbilities[Index];
	}

	// Return nullptr or a default value if the index is invalid
	return nullptr;
}
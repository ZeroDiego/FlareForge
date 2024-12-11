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

void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
	bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
	return bIsMelee;
}

void UNetworkGameInstance::AddPlayerState(const FString& PlayerID, AMyPlayerState* PlayerState)
{
	if (!PlayerStatesMap.Contains(PlayerID))
	{
		PlayerStatesMap.Add(PlayerID, PlayerState);
		UE_LOG(LogTemp, Log, TEXT("Added Player %s to GameInstance"), *PlayerID);
	}
}

TArray<TSubclassOf<UGameplayAbility>> UNetworkGameInstance::GetAbilitiesForPlayer(const FString& PlayerID) const
{
	if (AMyPlayerState* const* FoundPlayerState = PlayerStatesMap.Find(PlayerID))
	{
		// Dereference FoundPlayerState to get the actual AMyPlayerState*
		const AMyPlayerState* PlayerState = *FoundPlayerState;

		// Now you can safely use PlayerState
		TArray<TSubclassOf<UGameplayAbility>> Abilities = PlayerState->GetSelectedAbilities();
	}
    
	return TArray<TSubclassOf<UGameplayAbility>>();
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer(const FString& PlayerID, const int32 Index) const
{
	// Check if the PlayerID exists in the PlayerStatesMap
	if (AMyPlayerState* const* FoundPlayerState = PlayerStatesMap.Find(PlayerID))
	{
		// Dereference FoundPlayerState to get the actual AMyPlayerState*
		const AMyPlayerState* PlayerState = *FoundPlayerState;

		// Retrieve the selected abilities for this player
		const TArray<TSubclassOf<UGameplayAbility>>& Abilities = PlayerState->GetSelectedAbilities();

		// Check if the index is valid
		if (Abilities.IsValidIndex(Index))
		{
			// Return the ability at the specified index
			return Abilities[Index];
		}
	}

	// Return nullptr if PlayerID is not found or index is invalid
	return nullptr;
}
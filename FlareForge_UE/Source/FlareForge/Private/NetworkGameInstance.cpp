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

/*void UNetworkGameInstance::SetSelectedAbilities_Implementation(const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities)
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
*/
void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
	bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
	return bIsMelee;
}

void UNetworkGameInstance::AddPlayerState(const FString& UniquePlayerID, AMyPlayerState* PlayerState)
{
	if (!PlayerStatesMap.Contains(UniquePlayerID))
	{
		PlayerStatesMap.Add(UniquePlayerID, PlayerState);
		UE_LOG(LogTemp, Log, TEXT("Added Player %s to GameInstance"), *UniquePlayerID);
	}
}

void UNetworkGameInstance::SetSelectedAbilitiesForPlayer_Implementation(const FString& UniquePlayerID, const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities)
{
	// Store the selected abilities for the given player ID
	PlayerSelectedAbilitiesMap.Add(UniquePlayerID, NewSelectedAbilities);

	// Debug output for verification
	for (const TSubclassOf<UGameplayAbility> Ability : NewSelectedAbilities)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EffectHandle %s for Player %s"), *Ability->GetName(), *UniquePlayerID));
	}
}

TArray<TSubclassOf<UGameplayAbility>> UNetworkGameInstance::GetAbilitiesForPlayer(const FString& UniquePlayerID) const
{
	// Check if the player ID exists in the map
	if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
	{
		return *FoundAbilities;
	}

	// Return an empty array if no abilities are found for the player ID
	return TArray<TSubclassOf<UGameplayAbility>>();
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const
{
	// Retrieve the abilities list for the given player ID
	if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
	{
		// Check if the index is valid
		if (FoundAbilities->IsValidIndex(Index))
		{
			return (*FoundAbilities)[Index];
		}
	}

	// Return nullptr if no abilities are found or the index is invalid
	return nullptr;
}
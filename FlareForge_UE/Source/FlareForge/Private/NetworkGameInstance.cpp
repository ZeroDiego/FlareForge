// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

void UNetworkGameInstance::SetGameplayAbilitySpecAtIndex_Implementation(const FString& UniquePlayerID, const FGameplayAbilitySpec& NewGameplayAbilitySpec, int32 AtIndex)
{
	if (!PlayerAbilitySpecsMap.Contains(UniquePlayerID))
	{
		PlayerAbilitySpecsMap.Add(UniquePlayerID, TArray<FGameplayAbilitySpec>());
	}

	TArray<FGameplayAbilitySpec>& AbilitySpecs = PlayerAbilitySpecsMap[UniquePlayerID];
	if (AtIndex >= 0 && AtIndex < AbilitySpecs.Num())
	{
		AbilitySpecs[AtIndex] = NewGameplayAbilitySpec;
	}
	else
	{
		AbilitySpecs.SetNum(AtIndex + 1);
		AbilitySpecs[AtIndex] = NewGameplayAbilitySpec;
	}
}


TArray<FGameplayAbilitySpec> UNetworkGameInstance::GetGameplayAbilitySpec(const FString& UniquePlayerID) const
{
	if (const TArray<FGameplayAbilitySpec>* FoundAbilities = PlayerAbilitySpecsMap.Find(UniquePlayerID))
	{
		return *FoundAbilities;
	}
	return TArray<FGameplayAbilitySpec>();
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

	/*// Debug output for verification
	for (const TSubclassOf<UGameplayAbility> Ability : NewSelectedAbilities)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EffectHandle %s for Player %s"), *Ability->GetName(), *UniquePlayerID));
	}*/
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

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer_Implementation(const FString& UniquePlayerID, const int32 Index) const
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

void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
	bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
	return bIsMelee;
}
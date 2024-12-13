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

void UNetworkGameInstance::GetAbilityAtIndexForPlayer_Implementation(const FString& UniquePlayerID, const int32 Index)
{
	if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
	{
		if (FoundAbilities->IsValidIndex(Index))
		{
			// Call client RPC to send back the ability
			ClientReceiveAbility(UniquePlayerID, (*FoundAbilities)[Index]);
			return;
		}
	}
	// Call client RPC with nullptr if index is invalid or abilities not found
	ClientReceiveAbility(UniquePlayerID, nullptr);
}

bool UNetworkGameInstance::GetAbilityAtIndexForPlayer_Validate(const FString& UniquePlayerID, const int32 Index)
{
	return true; // Add any necessary validation logic here
}

void UNetworkGameInstance::ClientReceiveAbility_Implementation(const FString& UniquePlayerID, TSubclassOf<UGameplayAbility> Ability)
{
	if (Ability)
	{
		UE_LOG(LogTemp, Log, TEXT("Received Ability %s for Player %s"), *Ability->GetName(), *UniquePlayerID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Ability found at given index for Player %s"), *UniquePlayerID);
	}
}

void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
	bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
	return bIsMelee;
}
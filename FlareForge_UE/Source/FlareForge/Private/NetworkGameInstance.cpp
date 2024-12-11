// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"
#include "Engine/Engine.h"

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

void UNetworkGameInstance::AddPlayerState(const FString& UniquePlayerID, AMyPlayerState* PlayerState)
{
	if (!PlayerStatesMap.Contains(UniquePlayerID))
	{
		PlayerStatesMap.Add(UniquePlayerID, PlayerState);
		UE_LOG(LogTemp, Log, TEXT("Added Player %s to GameInstance"), *UniquePlayerID);
	}
}

TArray<TSubclassOf<UGameplayAbility>> UNetworkGameInstance::GetAbilitiesForPlayer(const FString& UniquePlayerID) const
{
	if (AMyPlayerState* const* FoundPlayerState = PlayerStatesMap.Find(UniquePlayerID))
	{
		// Dereference FoundPlayerState to get the actual AMyPlayerState*
		const AMyPlayerState* PlayerState = *FoundPlayerState;

		// Now you can safely use PlayerState
		TArray<TSubclassOf<UGameplayAbility>> Abilities = PlayerState->GetSelectedAbilities();
	}
    
	return TArray<TSubclassOf<UGameplayAbility>>();
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const
{
	if (AMyPlayerState* const* FoundPlayerState = PlayerStatesMap.Find(UniquePlayerID))
	{
		const AMyPlayerState* PlayerState = *FoundPlayerState;
		return PlayerState->GetAbilityAtIndex(Index);
	}

	return nullptr;
}

FString UNetworkGameInstance::GenerateUniquePlayerId()
{
	FString NewId;
	do
	{
		NewId = FGuid::NewGuid().ToString();
	} while (UsedPlayerIds.Contains(NewId));

	UsedPlayerIds.Add(NewId);
	return NewId;
	
	// Print debug message if GEngine is available
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("Assigned Unique ID: %s"), *NewId));
	}
}

void UNetworkGameInstance::AssignPlayerId(APlayerController* PlayerController)
{
	if (AMyPlayerState* PS = Cast<AMyPlayerState>(PlayerController->PlayerState))
	{
		FString PlayerID = GenerateUniquePlayerId();
		PS->SetUniquePlayerId(PlayerID);
	}
}
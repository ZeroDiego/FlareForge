// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"
#include "Net/UnrealNetwork.h"

void UNetworkGameInstance::SetGameplayAbilitySpecAtIndex_Implementation(const FString& UniquePlayerID, const FGameplayAbilitySpec& NewGameplayAbilitySpec, int32 AtIndex)
{
    if (!PlayerAbilitySpecsMap.Contains(UniquePlayerID))
        PlayerAbilitySpecsMap.Add(UniquePlayerID, TArray<FGameplayAbilitySpec>());

    TArray<FGameplayAbilitySpec>& AbilitySpecs = PlayerAbilitySpecsMap[UniquePlayerID];
    
    if (AtIndex >= 0 && AtIndex < AbilitySpecs.Num())
        AbilitySpecs[AtIndex] = NewGameplayAbilitySpec;
    else
        AbilitySpecs.SetNum(AtIndex + 1);
        AbilitySpecs[AtIndex] = NewGameplayAbilitySpec;
}

TArray<FGameplayAbilitySpec> UNetworkGameInstance::GetGameplayAbilitySpec(const FString& UniquePlayerID) const
{
    if (const TArray<FGameplayAbilitySpec>* FoundAbilities = PlayerAbilitySpecsMap.Find(UniquePlayerID))
    {
        if (GEngine)
        {
            /*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT("Found %d Abilities for Player ID: %s"), FoundAbilities->Num(), *UniquePlayerID));*/
        }
        return *FoundAbilities;
    }

    return TArray<FGameplayAbilitySpec>();
}

void UNetworkGameInstance::AddPlayerState_Implementation(const FString& UniquePlayerID, const FString& PlayerState)
{
    if (!PlayerStatesMap.Contains(UniquePlayerID))
        PlayerStatesMap.Add(UniquePlayerID, PlayerState);

    // Update the replicated array
    FPlayerStatePair NewPair;
    NewPair.PlayerID = UniquePlayerID;
    NewPair.PlayerState = PlayerState;
    PlayerStatesArray.Add(NewPair);

    //UE_LOG(LogTemp, Log, TEXT("Added Player %s with State %s to GameInstance"), *UniquePlayerID, *PlayerState);
}

void UNetworkGameInstance::SetSelectedAbilitiesForPlayer_Implementation(const FString& UniquePlayerID, const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities)
{
   PlayerSelectedAbilitiesMap.Add(UniquePlayerID, NewSelectedAbilities);
   /*
   for (const TSubclassOf<UGameplayAbility> Ability : NewSelectedAbilities)
       GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
           FString::Printf(TEXT("EffectHandle %s for Player %s"), *Ability->GetName(), *UniquePlayerID));*/
}

TArray<TSubclassOf<UGameplayAbility>> UNetworkGameInstance::GetAbilitiesForPlayer(const FString& UniquePlayerID) const
{
   if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
       return *FoundAbilities;

   return TArray<TSubclassOf<UGameplayAbility>>();
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const
{
   if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
       if (FoundAbilities->IsValidIndex(Index))
           return (*FoundAbilities)[Index];

   return nullptr;
}

FString UNetworkGameInstance::GetUniquePlayerIDFromState(const FString& PlayerState) const
{
    for (const auto& Entry : PlayerStatesMap)
        if (Entry.Value == PlayerState)
            return Entry.Key;

    return FString();
}

FString UNetworkGameInstance::GetPlayerNameFromState(const FString& UniquePlayerID) const
{
    
    if (const FString* PlayerState = PlayerStatesMap.Find(UniquePlayerID))
    {
        return *PlayerState; // Directly return the PlayerState string
    }
    return FString("Unknown");
}

void UNetworkGameInstance::PrintPlayerScores(const FString UniquePlayerID)
{
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow,
                                    FString::Printf(TEXT("%s score: %d"), *UniquePlayerID, *PlayerScores.Find(UniquePlayerID)));
}

void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
   bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
   return bIsMelee;
}

void UNetworkGameInstance::OnRep_PlayerStatesArray()
{
    // Rebuild local map from replicated array
    PlayerStatesMap.Empty();
    for (const FPlayerStatePair& Pair : PlayerStatesArray)
        PlayerStatesMap.Add(Pair.PlayerID, Pair.PlayerState);
}

void UNetworkGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   DOREPLIFETIME(UNetworkGameInstance, PlayerStatesArray);
}
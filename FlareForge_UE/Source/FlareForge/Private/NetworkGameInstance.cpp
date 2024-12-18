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
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT("Found %d Abilities for Player ID: %s"), FoundAbilities->Num(), *UniquePlayerID));
        }
        return *FoundAbilities;
    }

    return TArray<FGameplayAbilitySpec>();
}

void UNetworkGameInstance::SetSelectedAbilitiesForPlayer_Implementation(const FString& UniquePlayerID, const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities)
{
    // Find or add entry in the replicated array
    FPlayerAbilityData* ExistingEntry = ReplicatedAbilityDataArray.FindByPredicate([&](const FPlayerAbilityData& Data) {
        return Data.PlayerID == UniquePlayerID;
    });

    if (ExistingEntry)
    {
        ExistingEntry->Abilities = NewSelectedAbilities;
    }
    else
    {
        ReplicatedAbilityDataArray.Add(FPlayerAbilityData{UniquePlayerID, NewSelectedAbilities});
    }

    // Update local map for server-side use
    PlayerSelectedAbilitiesMap.Add(UniquePlayerID, NewSelectedAbilities);
}


TArray<TSubclassOf<UGameplayAbility>> UNetworkGameInstance::GetAbilitiesForPlayer(const FString& UniquePlayerID) const
{
   if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
       return *FoundAbilities;

   return TArray<TSubclassOf<UGameplayAbility>>();
}

TSubclassOf<UGameplayAbility> UNetworkGameInstance::GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT("GetAbilityAtIndexForPlayer")));
   if (const TArray<TSubclassOf<UGameplayAbility>>* FoundAbilities = PlayerSelectedAbilitiesMap.Find(UniquePlayerID))
   {
       GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT("Found %d Abilities for Player ID: %s"), FoundAbilities->Num(), *UniquePlayerID));
       if (FoundAbilities->IsValidIndex(Index))
       {
           GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT("Found %d Abilities for Player ID: %s"), FoundAbilities->Num(), *UniquePlayerID));
           return (*FoundAbilities)[Index];
       }
   }
    
   return nullptr;
}

void UNetworkGameInstance::AddReplicatedPlayerID_Implementation(const FString& UniquePlayerID)
{
    if (!ReplicatedPlayerIDs.Contains(UniquePlayerID))
    {
        ReplicatedPlayerIDs.Add(UniquePlayerID);
        OnRep_ReplicatedPlayerIDs();
        
        UE_LOG(LogTemp, Log, TEXT("Added Player ID: %s"), *UniquePlayerID);
    }
}

void UNetworkGameInstance::OnRep_ReplicatedPlayerIDs()
{
    PlayerIDs.Empty();
    for (const FString& PlayerID : ReplicatedPlayerIDs)
        PlayerIDs.Add(PlayerID);
}

void UNetworkGameInstance::OnRep_PlayerSelectedAbilities()
{
    PlayerSelectedAbilitiesMap.Empty();
    
    for (const FPlayerAbilityData& Data : ReplicatedAbilityDataArray)
    {
        PlayerSelectedAbilitiesMap.Add(Data.PlayerID, Data.Abilities);
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

void UNetworkGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(UNetworkGameInstance, ReplicatedPlayerIDs);
  DOREPLIFETIME(UNetworkGameInstance, ReplicatedAbilityDataArray);
}
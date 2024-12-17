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
        return *FoundAbilities;

    return TArray<FGameplayAbilitySpec>();
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

void UNetworkGameInstance::SetIsMelee(bool bNewIsMelee)
{
   bIsMelee = bNewIsMelee;
}

bool UNetworkGameInstance::GetIsMelee() const
{
   return bIsMelee;
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

void UNetworkGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UNetworkGameInstance, ReplicatedPlayerIDs);
}
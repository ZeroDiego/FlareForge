// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "LucasAbilitySystemComponent.h"
#include "FlareForge/Character/MyPlayerCharacter.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerStatePair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerID;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerState;
};

UCLASS()
class FLAREFORGE_API UNetworkGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGameplayAbilitySpec> GameplayAbilitySpec;

	UFUNCTION(Server, Reliable)
	void SetGameplayAbilitySpecAtIndex(const FString& UniquePlayerID, const FGameplayAbilitySpec& NewGameplayAbilitySpec, const int32 AtIndex);

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpec> GetGameplayAbilitySpec(const FString& UniquePlayerID) const;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Settings")
	bool bIsMelee;
	
	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	void SetIsMelee(bool bNewIsMelee);

	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	bool GetIsMelee() const;

	// Sets the selected abilities for a specific player identified by UniquePlayerID
	UFUNCTION(Server, Reliable)
	void SetSelectedAbilitiesForPlayer(const FString& UniquePlayerID, const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities);

	// Retrieves the list of abilities for a specific player identified by UniquePlayerID
	TArray<TSubclassOf<UGameplayAbility>> GetAbilitiesForPlayer(const FString& UniquePlayerID) const;

	// Retrieves a specific ability at a given index for a player identified by UniquePlayerID
	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	TSubclassOf<UGameplayAbility> GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const;

	// Add a new UniquePlayerID to the replicated list
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void AddReplicatedPlayerID(const FString& UniquePlayerID);
	void AddReplicatedPlayerID_Implementation(const FString& UniquePlayerID);
	bool AddReplicatedPlayerID_Validate(const FString& UniquePlayerID) { return true; }
	
	UPROPERTY(BlueprintReadWrite)
	TMap<AMyPlayerCharacter*, int> PlayerScores;

protected:
	
	UFUNCTION()
	void OnRep_ReplicatedPlayerIDs();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedPlayerIDs)
	TArray<FString> ReplicatedPlayerIDs;

	TSet<FString> PlayerIDs;
	
	// Map of Player IDs to their respective ability specs
	TMap<FString, TArray<FGameplayAbilitySpec>> PlayerAbilitySpecsMap;

	TMap<FString, TArray<TSubclassOf<UGameplayAbility>>> PlayerSelectedAbilitiesMap;
};

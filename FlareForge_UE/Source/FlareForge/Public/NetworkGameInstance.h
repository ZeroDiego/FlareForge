// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "LucasAbilitySystemComponent.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> SelectedAbilities;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Settings")
	bool bIsMelee;
	
	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	void SetIsMelee(bool bNewIsMelee);

	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	bool GetIsMelee() const;

	// Add a PlayerState to the map
	void AddPlayerState(const FString& UniquePlayerID, AMyPlayerState* PlayerState);

	// Sets the selected abilities for a specific player identified by UniquePlayerID
	UFUNCTION(Server, Reliable)
	void SetSelectedAbilitiesForPlayer(const FString& UniquePlayerID, const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities);

	// Retrieves the list of abilities for a specific player identified by UniquePlayerID
	TArray<TSubclassOf<UGameplayAbility>> GetAbilitiesForPlayer(const FString& UniquePlayerID) const;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Player Settings")
	void GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index);
	
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Player Settings")
	void ClientReceiveAbility(const FString& UniquePlayerID, TSubclassOf<UGameplayAbility> Ability);

private:
	// Map of Player IDs to PlayerStates
	UPROPERTY()
	TMap<FString, AMyPlayerState*> PlayerStatesMap;

	// Map of Player IDs to their respective ability specs
	TMap<FString, TArray<FGameplayAbilitySpec>> PlayerAbilitySpecsMap;

	TMap<FString, TArray<TSubclassOf<UGameplayAbility>>> PlayerSelectedAbilitiesMap;
};

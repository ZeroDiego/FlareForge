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

	/*UFUNCTION(Server, Reliable)
	void SetSelectedAbilities(const TArray<TSubclassOf<UGameplayAbility>>& NewSelectedAbilities);

	// Gets an ability from a specific index in SelectedAbilities
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayAbility> GetAbilityAtIndex(const int32 Index) const;
*/
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

	// Retrieves a specific ability at a given index for a player identified by UniquePlayerID
	UFUNCTION(BlueprintCallable, Category = "Player Settings")
	TSubclassOf<UGameplayAbility> GetAbilityAtIndexForPlayer(const FString& UniquePlayerID, const int32 Index) const;

private:
	// Map of Player IDs to PlayerStates
	TMap<FString, AMyPlayerState*> PlayerStatesMap;

	// Map of Player IDs to their respective ability specs
	TMap<FString, TArray<FGameplayAbilitySpec>> PlayerAbilitySpecsMap;

	TMap<FString, TArray<TSubclassOf<UGameplayAbility>>> PlayerSelectedAbilitiesMap;
};

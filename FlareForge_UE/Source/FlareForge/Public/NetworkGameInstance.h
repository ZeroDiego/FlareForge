// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "LucasAbilitySystemComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void SetGameplayAbilitySpecAtIndex(const FGameplayAbilitySpec NewGameplayAbilitySpec, const int32 AtIndex);

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpec> GetGameplayAbilitySpec() const;
};

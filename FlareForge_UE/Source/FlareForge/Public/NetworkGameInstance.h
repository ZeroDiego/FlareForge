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
	FGameplayAbilitySpec GameplayAbilitySpec;

	UFUNCTION(BlueprintCallable)
	void SetGameplayAbilitySpec(const FGameplayAbilitySpec NewGameplayAbilitySpec);

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpec GetGameplayAbilitySpec() const;
};

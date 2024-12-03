// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

void UNetworkGameInstance::SetGameplayAbilitySpecAtIndex(const FGameplayAbilitySpec NewGameplayAbilitySpec, const int32 AtIndex)
{
	GameplayAbilitySpec.Insert(NewGameplayAbilitySpec, AtIndex);
}

TArray<FGameplayAbilitySpec> UNetworkGameInstance::GetGameplayAbilitySpec() const
{
	return GameplayAbilitySpec;
}
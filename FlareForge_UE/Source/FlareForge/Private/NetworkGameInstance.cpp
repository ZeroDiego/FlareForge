// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

void UNetworkGameInstance::SetGameplayAbilitySpec(const FGameplayAbilitySpec NewGameplayAbilitySpec)
{
	GameplayAbilitySpec = NewGameplayAbilitySpec;
}

FGameplayAbilitySpec UNetworkGameInstance::GetGameplayAbilitySpec() const
{
	return GameplayAbilitySpec;
}
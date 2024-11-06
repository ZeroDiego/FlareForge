// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TeleportAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UTeleportAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere)
	float TeleportDistance = 1000.0f;
};

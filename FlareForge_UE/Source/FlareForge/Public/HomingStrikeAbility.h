// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HomingStrikeAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UHomingStrikeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> OrbProjectile;
	
	UPROPERTY(EditAnywhere)
	float CooldownTime;
	
	UPROPERTY(EditAnywhere)
	float DetectionRange;

	UPROPERTY(EditAnywhere)
	FVector SpawnOffset;

	UPROPERTY(EditAnywhere)
	float TravelSpeed;

	UPROPERTY(EditAnywhere)
	float TurnRate;

	UPROPERTY(EditAnywhere)
	float Width;
};

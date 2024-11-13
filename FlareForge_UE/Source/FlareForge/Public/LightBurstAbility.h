// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LightBurstAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API ULightBurstAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void LightBurstAbility(const FVector SpawnConeLocation, const FRotator CurrentRotation);

private:

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<ALightBurstCone> ConeBlueprint;

	//UPROPERTY(VisibleAnywhere)
	//ALightBurstCone* Cone;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightMeShield.h"
#include "Abilities/GameplayAbility.h"
#include "FIghtMeAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UFIghtMeAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	
	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void FightMeAbility(const FVector SpawnLocation, const FRotator CurrentRotation);

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActiveTime = 1.0f;*/
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFightMeShield> FightMeShieldBlueprint;

	UPROPERTY(VisibleAnywhere)
	AFightMeShield* FightMeShield;
};

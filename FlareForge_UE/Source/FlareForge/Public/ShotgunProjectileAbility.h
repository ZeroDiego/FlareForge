// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShotgunProjectile.h"
#include "Abilities/GameplayAbility.h"
#include "ShotgunProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UShotgunProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void ShotgunProjectileAbility(const FVector SpawnProjectileLocation, const FRotator CurrentRotation);
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShotgunProjectile> ShotgunProjectileBlueprint;

	UPROPERTY(VisibleAnywhere)
	AShotgunProjectile* ShotgunProjectile;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AnimDelay_Attack_Blueprint;
};

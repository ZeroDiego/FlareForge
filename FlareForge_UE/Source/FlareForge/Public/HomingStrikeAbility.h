// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FiringOffset.h"
#include "HomingStrikeProjectile.h"
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

	UFUNCTION(Server, Reliable)
	void HomingStrikeAbility(const FVector SpawnProjectileLocation, const FRotator CurrentRotation);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHomingStrikeProjectile> ProjectileBlueprint;

	UPROPERTY(VisibleAnywhere)
	AHomingStrikeProjectile* Projectile;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicProjectile.h"
#include "Abilities/GameplayAbility.h"
#include "BasicProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UBasicProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void BasicProjectileAbility(const FVector SpawnProjectileLocation, const FRotator CurrentRotation);
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasicProjectile> BasicProjectileBlueprint;

	UPROPERTY(VisibleAnywhere)
	ABasicProjectile* BasicProjectile;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectileHit.h"

void UBasicProjectileHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if(AActor* Actor = GetAvatarActorFromActorInfo())
	{
		
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
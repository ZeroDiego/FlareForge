// Fill out your copyright notice in the Description page of Project Settings.


#include "EmoteAbility.h"

#include "GameFramework/Character.h"

void UEmoteAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if(AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			Play_HelloEmote_Anim_OnServer(Character);
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEmoteAbility::Play_HelloEmote_Anim_OnServer_Implementation(ACharacter* Character)
{
	Character->PlayAnimMontage(HelloEmote_Anim);
}

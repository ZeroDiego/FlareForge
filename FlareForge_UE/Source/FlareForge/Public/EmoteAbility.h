// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmoteAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UEmoteAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void Play_HelloEmote_Anim_OnServer(ACharacter* Character);

	UFUNCTION(Server, Reliable)
	void Play_TBagEmote_Anim_OnServer(ACharacter* Character);
	
private:

	UPROPERTY(EditAnywhere)
	UAnimMontage* HelloEmote_Anim;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* TBagEmote_Anim;
};

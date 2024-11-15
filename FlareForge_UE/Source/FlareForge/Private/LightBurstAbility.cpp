// Fill out your copyright notice in the Description page of Project Settings.


#include "LightBurstAbility.h"
#include "FiringOffset.h"
#include "GameFramework/Character.h"

void ULightBurstAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnConeLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
			const FRotator CurrentRotation = Character->GetActorRotation();
			LightBurstAbility(SpawnConeLocation, CurrentRotation);
		}
	}
}

void ULightBurstAbility::LightBurstAbility_Implementation(const FVector SpawnConeLocation, const FRotator CurrentRotation)
{
	const FActorSpawnParameters SpawnParameters;
}
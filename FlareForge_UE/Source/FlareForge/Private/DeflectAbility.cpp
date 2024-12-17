// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectAbility.h"

#include "FiringOffset.h"
#include "ReflectOffset.h"
#include "GameFramework/Character.h"

void UDeflectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnDeflectShieldLocation = Character->GetComponentByClass<UReflectOffset>()->GetComponentLocation();
			const FRotator CurrentRotation = Character->GetActorRotation();
			DeflectAbility(SpawnDeflectShieldLocation, CurrentRotation);
			Shield->GetDeflectPlayerReference(Character->GetRootComponent());
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, AnimDelay_Reflect_Blueprint.GetDefaultObject(), 1.0f);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDeflectAbility::DeflectAbility_Implementation(const FVector SpawnDeflectShieldLocation, const FRotator CurrentRotation)
{
	const FActorSpawnParameters SpawnParameters;
	Shield = GetWorld()->SpawnActor<ADeflectShield>(ShieldBlueprint, SpawnDeflectShieldLocation, CurrentRotation, SpawnParameters);
}


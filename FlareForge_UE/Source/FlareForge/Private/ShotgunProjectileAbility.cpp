// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunProjectileAbility.h"
#include "GameFramework/Character.h"
#include "LucasAbilitySystemComponent.h"
#include "FiringOffset.h"

void UShotgunProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
			const FRotator CurrentRotation = Character->GetActorRotation();
			ShotgunProjectileAbility(SpawnProjectileLocation, CurrentRotation);
			//ShotgunProjectile->PlayerSpawningRootComponent(Character->GetRootComponent());
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	if (GEngine)
	{
		const FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, AnimDelay_Attack_Blueprint.GetDefaultObject(), 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EffectHandle %s"), *EffectHandle.ToString()));
	}
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UShotgunProjectileAbility::ShotgunProjectileAbility_Implementation(const FVector SpawnProjectileLocation, const FRotator CurrentRotation)
{
	const FActorSpawnParameters SpawnParameters;
	ShotgunProjectile = GetWorld()->SpawnActor<AShotgunProjectile>(ShotgunProjectileBlueprint, SpawnProjectileLocation, CurrentRotation, SpawnParameters);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeAbility.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UHomingStrikeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	// Make the Projectile Spawn
	if (const ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		const FVector SpawnProjectileLocation = Character->GetActorLocation() + SpawnOffset;
		const FRotator CurrentRotation = Character->GetActorRotation();
		const FActorSpawnParameters SpawnParameters;
		GetWorld()->SpawnActor<AActor>(OrbProjectile, SpawnProjectileLocation, CurrentRotation, SpawnParameters);
		CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	}
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeAbility.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UHomingStrikeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Actor))
		{
			if (const ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
				const FRotator CurrentRotation = Character->GetActorRotation();
				HomingStrikeAbility(SpawnProjectileLocation, CurrentRotation);
			}
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UHomingStrikeAbility::HomingStrikeAbility_Implementation(const FVector SpawnProjectileLocation, const FRotator CurrentRotation)
{
	// Make the Projectile Spawn
	const FActorSpawnParameters SpawnParameters;
	GetWorld()->SpawnActor<AHomingStrikeProjectile>(OrbProjectile, SpawnProjectileLocation, CurrentRotation, SpawnParameters);
	
}

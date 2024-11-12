// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeAbility.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "FiringOffset.h"
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
				Projectile->LockOn(Projectile->FindHomingStrikeTarget(), PlayerController->GetPawn()->GetRootComponent());
			}
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UHomingStrikeAbility::HomingStrikeAbility_Implementation(const FVector SpawnProjectileLocation, const FRotator CurrentRotation)
{
	const FActorSpawnParameters SpawnParameters;
	Projectile = GetWorld()->SpawnActor<AHomingStrikeProjectile>(ProjectileBlueprint, SpawnProjectileLocation, CurrentRotation, SpawnParameters);
}

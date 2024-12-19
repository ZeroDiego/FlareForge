// Fill out your copyright notice in the Description page of Project Settings.


#include "FIghtMeAbility.h"

#include "InterchangeResult.h"
#include "GameFramework/Character.h"

void UFIghtMeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Active Ability"));
	//UE_LOG(LogTemp, Warning, TEXT("acitvate"));
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
	
			const FVector SpawnShieldLocation = Character->GetActorLocation();
			const FRotator CurrentRotation = Character->GetActorRotation();

			FightMeAbility(SpawnShieldLocation, CurrentRotation);
			
			//FightMeShield->GetFightMePlayerReference(Character->GetRootComponent());
			
		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Fight Me"));
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFIghtMeAbility::FightMeAbility_Implementation(const FVector SpawnLocation, const FRotator CurrentRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("FightMeAbility_Implementation"));
	//const FActorSpawnParameters SpawnParameters;
	//FightMeShield = GetWorld()->SpawnActor<AFightMeShield>(FightMeShieldBlueprint, SpawnLocation, CurrentRotation, SpawnParameters);
}


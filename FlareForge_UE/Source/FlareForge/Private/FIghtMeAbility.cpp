// Fill out your copyright notice in the Description page of Project Settings.


#include "FIghtMeAbility.h"

#include "GameFramework/Character.h"

void UFIghtMeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("acitvate"));
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			//FightMeShield->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			//UE_LOG(LogTemp, Warning, TEXT("acitvate"));
			const FVector SpawnShieldLocation = Character->GetActorLocation();
			const FRotator CurrentRotation = Character->GetActorRotation();
			//const FActorSpawnParameters SpawnParameters;
			//FightMeShield = GetWorld()->SpawnActor<AFightMeShield>(FightMeShieldBlueprint, SpawnShieldLocation, CurrentRotation, SpawnParameters);
			//UE_LOG(LogTemp, Warning, TEXT("spawn shield"));
			FightMeAbility(SpawnShieldLocation, CurrentRotation);
		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Fight Me"));
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFIghtMeAbility::FightMeAbility_Implementation(const FVector SpawnLocation, const FRotator CurrentRotation)
{
	//UE_LOG(LogTemp, Display, TEXT("FightMeAbility_Implementation"));
	/*const FActorSpawnParameters SpawnParameters;
	FightMeShield = GetWorld()->SpawnActor<AFightMeShield>(FightMeShieldBlueprint, SpawnLocation, CurrentRotation, SpawnParameters);*/
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectAbility.h"

#include "FiringOffset.h"
#include "GameFramework/Character.h"

void UDeflectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			const FVector SpawnDeflectShieldLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
			UE_LOG(LogTemp, Warning, TEXT("forward vector: %s"), *Character->GetActorForwardVector().ToString());
			//const FVector CurrentVector = Character->GetActorForwardVector() * 2;
			const FRotator CurrentRotation = Character->GetActorRotation();
			//FName PlayerName = Character->GetFName();
			DeflectAbility(SpawnDeflectShieldLocation, CurrentRotation);
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDeflectAbility::DeflectAbility_Implementation(const FVector SpawnDeflectShieldLocation, const FRotator CurrentRotation)
{
	const FActorSpawnParameters SpawnParameters;
	Shield = GetWorld()->SpawnActor<ADeflectShield>(ShieldBlueprint, SpawnDeflectShieldLocation, CurrentRotation, SpawnParameters);
	/*if(Shield)
	{
		Shield->SetPlayerName(PlayerName);
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("Spawn shield"));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectileAbility.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UBasicProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	// Make the Character Teleport
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		Character->SetActorLocation(Character->GetActorLocation() + BasicProjectileVelocity, 1.0, nullptr, ETeleportType::ResetPhysics);
	}
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
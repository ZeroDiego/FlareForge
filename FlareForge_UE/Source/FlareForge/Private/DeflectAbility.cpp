// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectAbility.h"

UDeflectAbility::UDeflectAbility()
{
	/*DeflectBox = CreateDefaultSubobject<UBoxComponent>(FName("DeflectBox"));
	//RootComponent = DeflectBox;

	DeflectBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	DeflectBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DeflectBox->OnComponentBeginOverlap.AddDynamic(this, &UDeflectAbility::OnDeflectBoxOverlap);*/
}


void UDeflectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	/*if(AActor* Actor = GetAvatarActorFromActorInfo())
	{
		
	}*/
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"

void UMyAbilitySystemComponent::RecieveDamage(UMyAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	RecievedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
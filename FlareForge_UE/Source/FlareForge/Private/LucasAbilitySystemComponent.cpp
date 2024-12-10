// Fill out your copyright notice in the Description page of Project Settings.


#include "LucasAbilitySystemComponent.h"

ULucasAbilitySystemComponent::ULucasAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void ULucasAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

}
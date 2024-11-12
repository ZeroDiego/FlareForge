// Fill out your copyright notice in the Description page of Project Settings.


#include "LucasAbilitySystemComponent.h"

ULucasAbilitySystemComponent::ULucasAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void ULucasAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

}
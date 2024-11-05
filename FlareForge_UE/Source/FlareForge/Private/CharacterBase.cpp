// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "MyAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"

ACharacterBase::ACharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystemComponent");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//Give an attribute set to character
	if(IsValid(AbilitySystemComponent))
	{
		CharacterAttributeSet = AbilitySystemComponent->GetSet<UMyCharacterAttributeSet>();
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

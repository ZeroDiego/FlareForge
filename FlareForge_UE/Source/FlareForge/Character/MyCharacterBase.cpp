// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterBase.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"
#include "FlareForge/Character/MyPlayerState.h"


// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AMyCharacterBase::GetAbilitySystemComponent() const
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	if (MyPlayerState)
	{
		return MyPlayerState->GetAbilitySystemComponent();
	}
	return nullptr;
}

UMyCharacterAttributeSet* AMyCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

/*void AMyCharacterBase::GrantSelectedAbilities()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	if (MyPlayerState)
	{
		UAbilitySystemComponent* ASC = MyPlayerState->GetAbilitySystemComponent();
		if (ASC)
		{
			const TArray<TSubclassOf<UGameplayAbility>>& SelectedAbilities = MyPlayerState->GetSelectedAbilities();
			for (TSubclassOf<UGameplayAbility> AbilityClass : SelectedAbilities)
			{
				if (AbilityClass)
				{
					FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
					ASC->GiveAbility(AbilitySpec);
				}
			}
		}
	}
}*/

void AMyCharacterBase::InitDefaultAttributes() const
{
	if(!AbilitySystemComponent || !DefaultAttributeEffect) { return; }

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);

	if(SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());			
	}
}

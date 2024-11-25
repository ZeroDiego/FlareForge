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
	return AbilitySystemComponent;
}

UMyCharacterAttributeSet* AMyCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

const TArray<TSubclassOf<UGameplayAbility>>& AMyCharacterBase::GetSelectedAbilities() const {
	if (const AMyPlayerState* PS = GetPlayerState<AMyPlayerState>()) {
		return PS->SelectedAbilities;
	}

	static const TArray<TSubclassOf<UGameplayAbility>> EmptyArray;
	return EmptyArray; // Return an empty array if PlayerState is null
}

/*
void AMyCharacterBase::GiveSelectedAbilities()
{
const TArray<TSubclassOf<UGameplayAbility>>& AMyCharacterBase::GetSelectedAbilities() const {
	if (const AMyPlayerState* PS = GetPlayerState<AMyPlayerState>()) {
		return PS->SelectedAbilities;
	}

	static const TArray<TSubclassOf<UGameplayAbility>> EmptyArray;
	return EmptyArray; // Return an empty array if PlayerState is null
}
	}
}

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

void AMyCharacterBase::SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility)
{
	// Check if we have authority and if NewAbility is valid
	if (!HasAuthority() || !NewAbility)
	{
		return;
	}

	// Ensure that the index is within bounds
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Replace the existing ability at this index
		SelectedAbilities[Index] = NewAbility;
	}
	else
	{
		// resize the array to accommodate new index
		if (Index >= 0)
		{
			SelectedAbilities.SetNum(Index + 1);
			SelectedAbilities[Index] = NewAbility;
		}
	}
}

void AMyCharacterBase::RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove)
{
	// Check if AbilitySystemComponent is valid and we have authority
	if (!HasAuthority() || !AbilityToRemove)
	{
		return;
	}

	// Check if the ability exists in the SelectedAbilities array
	if (SelectedAbilities.Contains(AbilityToRemove))
	{
		// Remove the ability from the array
		SelectedAbilities.Remove(AbilityToRemove);
	}
}*/

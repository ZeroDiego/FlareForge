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
	bReplicates = true;
	bAlwaysRelevant = true;
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

void AMyCharacterBase::InitAbilitySystemComponent()
{
	if (AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>())
	{
		AbilitySystemComponent = Cast<ULucasAbilitySystemComponent>(MyPlayerState->GetAbilitySystemComponent());
		if (!AbilitySystemComponent)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to cast AbilitySystemComponent to ULucasAbilitySystemComponent"));
			}
			return;
		}

		AttributeSet = MyPlayerState->GetAttributeSet();

		// Initialize ASC with character as avatar
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);

			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Successfully initialized Ability System Component"));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to get PlayerState in InitAbilitySystemComponent"));
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

// Check if the character has a specific gameplay tag
bool AMyCharacterBase::HasMatchingGameplayTag(const FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->GetOwnedGameplayTags().HasTag(TagToCheck);
}

// Check if the character has any tags from the provided container
bool AMyCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->GetOwnedGameplayTags().HasAny(TagContainer);
}

// Check if the character has all tags from the provided container
bool AMyCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->GetOwnedGameplayTags().HasAll(TagContainer);
}

// Get all owned gameplay tags
void AMyCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = AbilitySystemComponent->GetOwnedGameplayTags();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerState.h"
#include "MyCharacterAttributeSet.h"
#include "CharacterGameplayAbility.h"
#include "MyAbilitySystemComponent.h"

ACharacterPlayerState::ACharacterPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMyCharacterAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMyCharacterAttributeSet* ACharacterPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool ACharacterPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ACharacterPlayerState::ShowAbiltyConfirmCancelText(bool ShowText)
{
	//Implement HUD
}

float ACharacterPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ACharacterPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ACharacterPlayerState::GetPower() const
{
	return AttributeSet->GetPower();
}

int32 ACharacterPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetLevel();
}

void ACharacterPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACharacterPlayerState::HealthChanged);
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACharacterPlayerState::MaxHealthChanged);
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACharacterPlayerState::CharacterLevelChanged);
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACharacterPlayerState::PowerChanged);
	}
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACharacterPlayerState::StunTagChanged);
}

void ACharacterPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}

void ACharacterPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxHealth Changed!"));

}

void ACharacterPlayerState::PowerChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Power Changed!"));

}

void ACharacterPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterLevel Changed!"));

}


void ACharacterPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}





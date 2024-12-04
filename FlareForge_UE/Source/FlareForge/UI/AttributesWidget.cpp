// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "MyCharacterAttributeSet.h"
#include "FlareForge/Character/MyPlayerState.h"

void UAttributesWidget::BindToAttributes(const AMyPlayerState* MyPlayerState)
{
	if(!MyPlayerState)
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	const UMyCharacterAttributeSet* FlareForgeAttributeSet = MyPlayerState->GetAttributeSet();

	// Initial Attributes
	HealthPercent = NUMERIC_VALUE(FlareForgeAttributeSet, Health) / NUMERIC_VALUE(FlareForgeAttributeSet, MaxHealth);

	// Attribute Changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FlareForgeAttributeSet->GetHealthAttribute()).AddLambda(
	[this, FlareForgeAttributeSet](const FOnAttributeChangeData& Data)
	{
		HealthPercent = Data.NewValue / NUMERIC_VALUE(FlareForgeAttributeSet, MaxHealth);
	});
}
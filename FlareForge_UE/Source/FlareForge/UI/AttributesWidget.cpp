// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "MyCharacterAttributeSet.h"
#include "FlareForge/Character/MyPlayerState.h"

void UAttributesWidget::BindToAttributes(){

const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetOwningPlayerState());
	if(!MyPlayerState) return;

	UAbilitySystemComponent* ASC = MyPlayerState->GetAbilitySystemComponent();
	const UMyCharacterAttributeSet* FlareForgeAS = MyPlayerState->GetAttributeSet();

	// Initial Attributes
	HealthPercent = NUMERIC_VALUE(FlareForgeAS, Health) / NUMERIC_VALUE(FlareForgeAS, MaxHealth);

	// Attribute Changes
	ASC->GetGameplayAttributeValueChangeDelegate(FlareForgeAS->GetHealthAttribute()).AddLambda(
	[this, FlareForgeAS](const FOnAttributeChangeData& Data)
	{
		HealthPercent = Data.NewValue / NUMERIC_VALUE(FlareForgeAS, MaxHealth);
	});
}
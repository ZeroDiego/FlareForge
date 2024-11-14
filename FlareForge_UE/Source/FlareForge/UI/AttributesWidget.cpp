// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "MyCharacterAttributeSet.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "Kismet/GameplayStatics.h"

void UAttributesWidget::BindToAttributes()
{
	const AMyPlayerState* MyPlayerState1 = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if(!MyPlayerState1) return;

	UAbilitySystemComponent* ASC1 = MyPlayerState1->GetAbilitySystemComponent();
	const UMyCharacterAttributeSet* FlareForgeAS1 = MyPlayerState1->GetAttributeSet();

	// Initial Attributes
	HealthPercent = NUMERIC_VALUE(FlareForgeAS1, Health) / NUMERIC_VALUE(FlareForgeAS1, MaxHealth);

	// Attribute Changes
	ASC1->GetGameplayAttributeValueChangeDelegate(FlareForgeAS1->GetHealthAttribute()).AddLambda(
	[this, FlareForgeAS1](const FOnAttributeChangeData& Data)
	{
		HealthPercent = Data.NewValue / NUMERIC_VALUE(FlareForgeAS1, MaxHealth);
	});
	const AMyPlayerState* MyPlayerState2 = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 1));
	if(!MyPlayerState2) return;

	UAbilitySystemComponent* ASC2 = MyPlayerState2->GetAbilitySystemComponent();
	const UMyCharacterAttributeSet* FlareForgeAS2 = MyPlayerState2->GetAttributeSet();

	// Initial Attributes
	Player2HealthPercent = NUMERIC_VALUE(FlareForgeAS2, Health) / NUMERIC_VALUE(FlareForgeAS2, MaxHealth);

	// Attribute Changes
	ASC2->GetGameplayAttributeValueChangeDelegate(FlareForgeAS2->GetHealthAttribute()).AddLambda(
	[this, FlareForgeAS2](const FOnAttributeChangeData& Data)
	{
		Player2HealthPercent = Data.NewValue / NUMERIC_VALUE(FlareForgeAS2, MaxHealth);
	});
}
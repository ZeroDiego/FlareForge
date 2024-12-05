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

void UAttributesWidget::NativeTick(const FGeometry& MyGeometry, float DeltaSeconds)
{
	Super::NativeTick(MyGeometry, DeltaSeconds);

	const AMyPlayerState* MyPlayerState = GetOwningLocalPlayer()->GetPlayerController(GetWorld())->GetPlayerState<AMyPlayerState>();
	
	if(!MyPlayerState)
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();

	const FActiveGameplayEffectsContainer& ActiveEffects = AbilitySystemComponent->GetActiveGameplayEffects();
	
	// Use the custom iterator provided by FActiveGameplayEffectsContainer
	for (FActiveGameplayEffectsContainer::ConstIterator It = ActiveEffects.CreateConstIterator(); It; ++It)
	{
		const FActiveGameplayEffect& Effect = *It;

		// Compare the effect's class with the desired class
		if (Effect.Spec.Def->GetClass() == BasicCooldown)
		{
			const float BasicCooldownDuration = AbilitySystemComponent->GetGameplayEffectDuration(Effect.Handle);
			BasicPercent = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle) / BasicCooldownDuration;
			BasicCooldownTime = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle);
		}
	}
}

float UAttributesWidget::GetRemainingEffectTime(UAbilitySystemComponent* AbilitySystemComponent, FActiveGameplayEffectHandle EffectHandle)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is null."));
		return -1.0f;
	}

	const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(EffectHandle);

	if (ActiveEffect)
	{
		const float CurrentTime = AbilitySystemComponent->GetWorld()->GetTimeSeconds();
		const float EffectStartTime = ActiveEffect->StartWorldTime;
		const float EffectDuration = ActiveEffect->Spec.GetDuration();

		if (EffectDuration > 0.0f) // Check if the effect has a finite duration
		{
			float RemainingTime = (EffectStartTime + EffectDuration) - CurrentTime;
			return FMath::Max(RemainingTime, 0.0f); // Ensure it's not negative
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Effect has infinite duration."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find active effect with the specified handle."));
	}

	return -1.0f; // Return -1 if the effect is not found or has infinite duration
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "MyCharacterAttributeSet.h"
#include "FlareForge/Character/MyPlayerState.h"

void UAttributesWidget::BindToAttributes(const AMyPlayerState* MyPlayerState)
{
	if(!MyPlayerState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MyPlayerState: NULL")));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MyPlayerState: Correct")));
	
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

void UAttributesWidget::NativeTick(const FGeometry& MyGeometry, const float DeltaSeconds)
{
	Super::NativeTick(MyGeometry, DeltaSeconds);

	const AMyPlayerState* MyPlayerState = GetOwningLocalPlayer()->GetPlayerController(GetWorld())->GetPlayerState<AMyPlayerState>();
	
	if(!MyPlayerState)
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();

	const FActiveGameplayEffectsContainer& ActiveEffects = AbilitySystemComponent->GetActiveGameplayEffects();
	
	// Use the custom iterator provided by FActiveGameplayEffectsContainer
	for (FActiveGameplayEffectsContainer::ConstIterator It = ActiveEffects.CreateConstIterator(); It; ++It)
	{
		const FActiveGameplayEffect& Effect = *It;

		FNumberFormattingOptions FormattingOptions;
		FormattingOptions.MinimumIntegralDigits = 1;  // Minimum whole digits
		FormattingOptions.MaximumIntegralDigits = 2; // Maximum whole digits
		FormattingOptions.MinimumFractionalDigits = 1; // Minimum decimal places
		FormattingOptions.MaximumFractionalDigits = 1; // Maximum decimal places
		
		// Compare the effect's class with the desired class
		if (Effect.Spec.Def->GetClass() == BasicCooldown)
		{
			const float BasicCooldownDuration = AbilitySystemComponent->GetGameplayEffectDuration(Effect.Handle);
			BasicPercent = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle) / BasicCooldownDuration;
			const float BasicCooldownTime = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle);
			BasicCooldownTimeText = FText::AsNumber(BasicCooldownTime, &FormattingOptions);
		}

		if (Effect.Spec.Def->GetClass() == HomingCooldown)
		{
			const float HomingCooldownDuration = AbilitySystemComponent->GetGameplayEffectDuration(Effect.Handle);
			HomingPercent = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle) / HomingCooldownDuration;
			const float HomingCooldownTime = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle);
			HomingCooldownTimeText = FText::AsNumber(HomingCooldownTime, &FormattingOptions);
		}

		if (Effect.Spec.Def->GetClass() == ReflectCooldown)
		{
			const float ReflectCooldownDuration = AbilitySystemComponent->GetGameplayEffectDuration(Effect.Handle);
			ReflectPercent = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle) / ReflectCooldownDuration;
			const float ReflectCooldownTime = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle);
			ReflectCooldownTimeText = FText::AsNumber(ReflectCooldownTime, &FormattingOptions);
		}

		if (Effect.Spec.Def->GetClass() == DashCooldown)
		{
			const float DashCooldownDuration = AbilitySystemComponent->GetGameplayEffectDuration(Effect.Handle);
			DashPercent = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle) / DashCooldownDuration;
			const float DashCooldownTime = GetRemainingEffectTime(AbilitySystemComponent, Effect.Handle);
			DashCooldownTimeText = FText::AsNumber(DashCooldownTime, &FormattingOptions);
		}
	}
}

float UAttributesWidget::GetRemainingEffectTime(const UAbilitySystemComponent* AbilitySystemComponent, const FActiveGameplayEffectHandle EffectHandle)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is null."));
		return -1.0f;
	}

	if (const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(EffectHandle))
	{
		const float CurrentTime = AbilitySystemComponent->GetWorld()->GetTimeSeconds();
		const float EffectStartTime = ActiveEffect->StartWorldTime;

		if (const float EffectDuration = ActiveEffect->Spec.GetDuration(); EffectDuration > 0.0f) // Check if the effect has a finite duration
		{
			const float RemainingTime = (EffectStartTime + EffectDuration) - CurrentTime;
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
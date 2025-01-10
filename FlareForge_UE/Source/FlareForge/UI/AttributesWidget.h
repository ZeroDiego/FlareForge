// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicAbilityCooldown.h"
#include "Blueprint/UserWidget.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "UObject/Object.h"
#include "AttributesWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BindToAttributes(const AMyPlayerState* MyPlayerState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthPercentChanged();

	static float GetRemainingEffectTime(const UAbilitySystemComponent* AbilitySystemComponent, FActiveGameplayEffectHandle EffectHandle);
	
	// Called every frame.
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	float HealthPercent;

	UPROPERTY(BlueprintReadOnly)
	float BasicPercent;

	UPROPERTY(BlueprintReadOnly)
	FText BasicCooldownTimeText;

	UPROPERTY(BlueprintReadOnly)
	float HomingPercent;

	UPROPERTY(BlueprintReadOnly)
	FText HomingCooldownTimeText;

	UPROPERTY(BlueprintReadOnly)
	float ReflectPercent;

	UPROPERTY(BlueprintReadOnly)
	FText ReflectCooldownTimeText;

	UPROPERTY(BlueprintReadOnly)
	float DashPercent;

	UPROPERTY(BlueprintReadOnly)
	FText DashCooldownTimeText;
	
	UPROPERTY(BlueprintReadOnly)
	float FightMePercent;

	UPROPERTY(BlueprintReadOnly)
	FText FightMeCooldownTimeText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> BasicCooldown;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> HomingCooldown;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> ReflectCooldown;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DashCooldown;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> FightMeCooldown;
};

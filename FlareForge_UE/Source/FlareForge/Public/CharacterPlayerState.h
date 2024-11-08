// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "MyCharacterAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "CharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API ACharacterPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UMyCharacterAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState - UI")
	void ShowAbiltyConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState - Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState - Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState - Attributes")
	float GetPower() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterPlayerState - Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UMyAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UMyCharacterAttributeSet* AttributeSet;
	
	FGameplayTag DeadTag;
	
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle LevelChangedDelegateHandle;
	FDelegateHandle PowerChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void PowerChanged(const FOnAttributeChangeData& Data);
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);



};
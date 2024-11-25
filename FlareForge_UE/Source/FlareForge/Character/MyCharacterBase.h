// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MyCharacterBase.generated.h"

class UGameplayAbility;
class ULucasAbilitySystemComponent;
class UMyCharacterAttributeSet;
class UGameplayEffect;

UCLASS()
class FLAREFORGE_API AMyCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyCharacterBase();
	//~IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface interface
	virtual UMyCharacterAttributeSet* GetAttributeSet() const;
	
	const TArray<TSubclassOf<UGameplayAbility>>& GetSelectedAbilities() const;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (ExposeOnSpawn=true))
	TArray<TSubclassOf<UGameplayAbility>> SelectedAbilities;
	*/
	/*// Adds an ability to the SelectedAbilities array
	// Sets an ability at a specific index in the SelectedAbilities array
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility);

	// Removes an ability from the SelectedAbilities array
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove);*/
	
protected:
	//void GiveSelectedAbilities();
	//void InitDefaultAttributes() const;

	UPROPERTY()
	TObjectPtr<ULucasAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMyCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
};
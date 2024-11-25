// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "MyPlayerState.generated.h"

class UGameplayAbility;
class ULucasAbilitySystemComponent;
class UMyCharacterAttributeSet;
/**
 * 
 */
UCLASS()
class FLAREFORGE_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyPlayerState();

	// Pending abilities to be granted to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> SelectedAbilities;

	// Adds or sets an ability at a specific index in SelectedAbilities
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility);

	// Removes an ability from SelectedAbilities
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove);

	// Transfers SelectedAbilities to the Ability System Component (ASC)
	void TransferAbilitiesToASC();
	
	//~IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface interface
	virtual UMyCharacterAttributeSet* GetAttributeSet() const;

protected:	
	UPROPERTY()
	TObjectPtr<ULucasAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMyCharacterAttributeSet> AttributeSet;
};

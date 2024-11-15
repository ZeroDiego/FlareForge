// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "MyPlayerState.generated.h"

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

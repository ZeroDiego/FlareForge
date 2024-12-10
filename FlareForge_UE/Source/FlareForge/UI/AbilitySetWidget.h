// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySetWidget.generated.h"

class AMyPlayerState;
/**
 * 
 */
UCLASS()
class FLAREFORGE_API UAbilitySetWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetAbilityInPlayerState(int32 Index, TSubclassOf<UGameplayAbility> NewAbility);
	
	// Sets reference to PlayerState
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetPlayerStateReference(AMyPlayerState* PlayerStateRef);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AMyPlayerState* PlayerStateReference;
};

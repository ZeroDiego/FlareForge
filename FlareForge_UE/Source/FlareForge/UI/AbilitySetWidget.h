// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySetWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UAbilitySetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets the reference to FlareForgePlayerController
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetControllerReference(AFlareForgePlayerController* ControllerRef);

protected:
	// Reference to FlareForgePlayerController
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AFlareForgePlayerController* ControllerReference;
};

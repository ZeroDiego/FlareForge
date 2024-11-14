// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void BindToAttributes();

protected:
	UPROPERTY(BlueprintReadOnly)
	float HealthPercent;
};

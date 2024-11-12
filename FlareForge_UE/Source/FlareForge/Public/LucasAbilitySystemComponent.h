// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LucasAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class FLAREFORGE_API ULucasAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	ULucasAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
};
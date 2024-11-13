// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FlareForgeHUD.generated.h"


class UAttributesWidget;
/**
 * 
 */
UCLASS()
class FLAREFORGE_API AFlareForgeHUD : public AHUD
{
	GENERATED_BODY()
	
	public:
	void Init();
	
	protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributesWidget> AttributeWidgetClass;
	
	private:
	UPROPERTY()
	TObjectPtr<UAttributesWidget> AttributeWidget;
};

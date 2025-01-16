// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "FlareForgeGameMode.generated.h"

UCLASS(minimalapi)
class AFlareForgeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFlareForgeGameMode();

	UFUNCTION(BlueprintCallable)
	void NonSeamlessServerTravel(const FString& InURL);
};



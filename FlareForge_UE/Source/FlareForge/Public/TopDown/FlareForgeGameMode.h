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

	// Override PostLogin to assign unique IDs
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	// Generate a unique player ID
	FString GenerateUniquePlayerId();

	// Track used player IDs to ensure uniqueness
	TSet<FString> UsedPlayerIds;};




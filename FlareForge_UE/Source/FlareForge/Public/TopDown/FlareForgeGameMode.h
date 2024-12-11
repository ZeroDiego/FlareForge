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

private:
	TSet<FString> UsedPlayerIds; // Track used IDs
	
public:
	AFlareForgeGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	FString GenerateUniquePlayerId();
};




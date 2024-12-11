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

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int32 CurrentPlayerIndex = 0; // Tracks player indices (e.g., C0, C1)
};




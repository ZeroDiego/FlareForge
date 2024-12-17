// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AbilitySelectGamemode.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API AAbilitySelectGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Override PostLogin to assign unique IDs
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	// Generate a unique player ID
	FString GenerateUniquePlayerId();

	// Track used player IDs to ensure uniqueness
	TSet<FString> UsedPlayerIds;
};

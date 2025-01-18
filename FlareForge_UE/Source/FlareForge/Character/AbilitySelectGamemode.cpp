// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySelectGamemode.h"
#include "FlareForge/Character/MyPlayerState.h"

void AAbilitySelectGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Cast to custom PlayerState
	if (AMyPlayerState* PS = Cast<AMyPlayerState>(NewPlayer->PlayerState))
	{
		// Generate a unique ID and assign it to the PlayerState
		FString PlayerID = GenerateUniquePlayerId();
		PS->SetUniquePlayerId(PlayerID);

		// Debug log and screen message for verification
		/*UE_LOG(LogTemp, Log, TEXT("Assigned Unique ID: %s"), *PlayerID);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Assigned Unique ID: %s"), *PlayerID));
		}*/
	}
}

FString AAbilitySelectGamemode::GenerateUniquePlayerId()
{
	if (!UsedPlayerIds.Contains("Player 1"))
	{
		UsedPlayerIds.Add("Player 1");
		return "Player 1";
	}
	if (!UsedPlayerIds.Contains("Player 2"))
	{
		UsedPlayerIds.Add("Player 2");
		return "Player 2";
	}
	if (!UsedPlayerIds.Contains("Player 3"))
	{
		UsedPlayerIds.Add("Player 3");
		return "Player 3";
	}
	if (!UsedPlayerIds.Contains("Player 4"))
	{
		UsedPlayerIds.Add("Player 4");
		return "Player 4";
	}
	
	FString NewId;
	
	do
	{
		NewId = FGuid::NewGuid().ToString(); // Generate a GUID
	} while (UsedPlayerIds.Contains(NewId)); // Ensure it's unique

	UsedPlayerIds.Add(NewId); // Track the generated ID
	return NewId;
}
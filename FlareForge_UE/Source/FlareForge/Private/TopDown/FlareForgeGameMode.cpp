// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgeGameMode.h"

#include "NetworkGameInstance.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "TopDown/FlareForgePlayerController.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "TopDown/FlareForgeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFlareForgeGameMode::AFlareForgeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFlareForgePlayerController::StaticClass();

	// set default HUD class to BP_FlareForgeHUD
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/UI/BP_FlareForgeHUD"));
	if (HUDClassFinder.Class != nullptr)
	{
		HUDClass = HUDClassFinder.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_MyPlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default Player State to BP_MyPlayerState
	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Game/TopDown/Blueprints/BP_MyPlayerState"));
	if (PlayerStateBPClass.Class != nullptr)
	{
		PlayerStateClass = PlayerStateBPClass.Class;
	}
	
	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
/*
void AFlareForgeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Cast to custom PlayerState
	if (AMyPlayerState* PS = Cast<AMyPlayerState>(NewPlayer->PlayerState))
	{
		// Generate a unique ID and assign it to the PlayerState
		FString PlayerID = GenerateUniquePlayerId();
		PS->SetUniquePlayerId(PlayerID);

		// Debug log and screen message for verification
		UE_LOG(LogTemp, Log, TEXT("Assigned Unique ID: %s"), *PlayerID);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Assigned Unique ID: %s"), *PlayerID));
		}
	}
}

FString AFlareForgeGameMode::GenerateUniquePlayerId()
{
	FString NewId;
	do
	{
		NewId = FGuid::NewGuid().ToString(); // Generate a GUID
	} while (UsedPlayerIds.Contains(NewId)); // Ensure it's unique

	UsedPlayerIds.Add(NewId); // Track the generated ID
	return NewId;
}*/
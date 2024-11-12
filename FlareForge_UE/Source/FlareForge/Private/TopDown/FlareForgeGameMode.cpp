// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgeGameMode.h"
#include "TopDown/FlareForgePlayerController.h"
#include "TopDown/FlareForgeCharacter.h"
#include "FlareForge/Character/MyPlayerCharacter.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AFlareForgeGameMode::AFlareForgeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFlareForgePlayerController::StaticClass();

	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/FlareForge/Blueprints/BP_MyPlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// Set default player state class
	PlayerStateClass = AMyPlayerState::StaticClass();
}

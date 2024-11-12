// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgeGameMode.h"
#include "TopDown/FlareForgePlayerController.h"
#include "TopDown/FlareForgeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFlareForgeGameMode::AFlareForgeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFlareForgePlayerController::StaticClass();
	
	// Reset the instance counter at the beginning of each play session
	AFlareForgePlayerController::InstanceCounter = 0;
	

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
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
}
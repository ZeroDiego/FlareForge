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

FString AFlareForgeGameMode::GenerateUniquePlayerId() {
	FString NewId;
	do {
		NewId = FGuid::NewGuid().ToString(); // Generate a GUID
	} while (UsedPlayerIds.Contains(NewId)); // Ensure it's unique

	UsedPlayerIds.Add(NewId); // Mark it as used
	return NewId;
}

void AFlareForgeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AMyPlayerState* PS = Cast<AMyPlayerState>(NewPlayer->GetPlayerState()))
	{
		// Generate and assign unique ID
		PS->PS_UniqueStringId = GenerateUniquePlayerId();
		PS->ForceNetUpdate();

		// Notify GameInstance about this player (optional)
		if (UWorld* World = GetWorld())
		{
			if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(World->GetGameInstance()))
			{
				NetworkGI->SetPlayerAbilities(PS->PS_UniqueStringId, TArray<TSubclassOf<UGameplayAbility>>());
			}
		}
	}
}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbility.h"
#include "TopDown/FlareForgePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDown/FlareForgeCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "LucasAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UTeleportAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		UE_LOG(LogTemp, Display, TEXT("Actor"));
		if (const AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(Actor))
		{
			UE_LOG(LogTemp, Display, TEXT("Playercontroller"));
			if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				UE_LOG(LogTemp, Display, TEXT("Character"));
				if (IsLocallyControlled())
				{
					UE_LOG(LogTemp, Display, TEXT("Mousething"));
					if (FVector MouseLocation, MouseDirection; PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
					{
						Server_ReceiveMouseData(MouseLocation, MouseDirection);
					}
				}

				if (HasAuthority(&ActivationInfo))
				{
					UE_LOG(LogTemp, Warning, TEXT("ActivateAbility called on %hd"), HasAuthority(&ActivationInfo));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ActivateAbility called on %hd"), HasAuthority(&ActivationInfo));
				}
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UTeleportAbility::Server_ReceiveMouseData_Implementation(FVector MouseLocation, FVector MouseDirection)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarActor is null"));
		return;
	}

	const AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(AvatarActor);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController cast failed"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character cast failed"));
		return;
	}
	
	FVector Start = MouseLocation;
	FVector End = MouseLocation + MouseDirection * 10000.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector HitLocation = HitResult.Location;
		Character->SetActorLocation(HitLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}
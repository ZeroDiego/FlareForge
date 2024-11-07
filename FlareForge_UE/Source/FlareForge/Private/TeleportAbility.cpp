// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportAbility.h"
//#include "TopDown/FlareForgePlayerController.h"
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
#include "MyAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UTeleportAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	// Make the Character Teleport
	/*if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		Character->SetActorLocation(Character->GetActorLocation() + TeleportDistance, 1.0, nullptr, ETeleportType::ResetPhysics);
	}*/

	TeleportAbility(GetAvatarActorFromActorInfo());

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UTeleportAbility::TeleportAbility_Implementation(const AActor* PlayerActor)
{
	UE_LOG(LogTemp, Display, TEXT("PlayerActor: %s"), *PlayerActor->GetName());
	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(Actor))
		{
			if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				FVector MouseLocation, MouseDirection;
				if (PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
				{
					// Ray trace to find the intersection with the plane the character is on
					FVector Start = MouseLocation;
					FVector End = MouseLocation + MouseDirection * 10000.0f;  // Extend the ray

					FHitResult HitResult;
					FCollisionQueryParams Params;
					Params.AddIgnoredActor(Character);
        
					if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
					{
						// Ensure the hit location is on the same plane as the character
						FVector HitLocation = HitResult.Location;
						Character->SetActorLocation(HitLocation, false, nullptr, ETeleportType::TeleportPhysics);
					}
				}
			}
		}
	}
}

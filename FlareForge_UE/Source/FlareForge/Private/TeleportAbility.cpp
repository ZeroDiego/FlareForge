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

	if (const AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Actor))
		{
			if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				const FVector TeleportDirection = Character->GetActorRotation().Vector();
				Character->SetActorLocation(TeleportDirection * TeleportDistance, true, nullptr, ETeleportType::ResetPhysics);

			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

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
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
		{
			if (IsLocallyControlled())
			{
				if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetOwner()))
				{
					if (FVector MouseLocation, MouseDirection; PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
					{
						Server_ReceiveMouseData(MouseLocation, MouseDirection);
					}
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

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UTeleportAbility::Server_ReceiveMouseData_Implementation(FVector MouseLocation, FVector MouseDirection)
{
	float OriginalTeleportDistance = TeleportDistance;
	
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarActor is null"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AvatarActor);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character cast failed"));
		return;
	}
	
	FVector TeleportVector = UKismetMathLibrary::GetForwardVector(Character->GetActorRotation());
	FVector TargetLocation = FVector(Character->GetActorLocation().X + TeleportVector.X * TeleportDistance,
	Character->GetActorLocation().Y + TeleportVector.Y * TeleportDistance, 100);
	//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), TeleportDistance);
	
	while(!Character->TeleportTo(TargetLocation, Character->GetActorRotation()))
	{
		TeleportDistance -= 50;
		TargetLocation = FVector(Character->GetActorLocation().X + TeleportVector.X * TeleportDistance,
			Character->GetActorLocation().Y + TeleportVector.Y * TeleportDistance, 100);
		Character->TeleportTo(TargetLocation, Character->GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), TeleportDistance);
	}

	TeleportDistance = OriginalTeleportDistance;
	
}
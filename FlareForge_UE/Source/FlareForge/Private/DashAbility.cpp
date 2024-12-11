// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAbility.h"
#include "TopDown//FlareForgePlayerController.h"
#include "TopDown/FlareForgeCharacter.h"
#include "GameFramework/Character.h"
#include "LucasAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (APawn* Pawn = Cast<APawn>(Actor))
		{
			if (AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(Pawn->GetController()))
			{
				//CallDashOnServer(PlayerController);
				//UE_LOG(LogTemp, Display, TEXT("Dash"));
			}
		}
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, AnimDelay_Dash_Blueprint.GetDefaultObject(), 1.0f);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}

/*void UDashAbility::CallDashOnServer_Implementation(AFlareForgePlayerController* PlayerController)
{
	PlayerController->Dash(DashSpeed);
}*/



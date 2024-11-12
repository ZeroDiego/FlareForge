// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAbility.h"
#include "TopDown//FlareForgePlayerController.h"
#include "GameFramework/Character.h"
#include "LucasAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	// Make the Character Dash
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//Character->SetActorLocation(Character->GetActorLocation() + DashDistance, 1.0, nullptr, ETeleportType::ResetPhysics);
		
		//Character->LaunchCharacter(DashDistance, )
	}

	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(Actor))
		{
			if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				//const FVector SpawnProjectileLocation = Character->GetComponentByClass<UFiringOffset>()->GetComponentLocation();
				//const FVector CurrentMovingDirection = Character->GetActorRotation().Vector();
				/*const FVector DashDirection = Character->GetActorRotation().Vector();
				Character->LaunchCharacter(DashDistance * DashDirection, false, false);*/
				
				/*const FVector CurrentMovingDirection = Character->GetCharacterMovement()->GetLastInputVector().GetSafeNormal();
				double MaxMoveSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
				FVector Dash = DashDistance * CurrentMovingDirection * MaxMoveSpeed;
				//HomingStrikeAbility(SpawnProjectileLocation, CurrentRotation);
				Character->LaunchCharacter(Dash, false, false);*/
			}
		}
	}


	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	
}
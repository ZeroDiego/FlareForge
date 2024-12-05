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
				PlayerController->Dash(DashSpeed);
			}
		}
	}
	
	/*if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		
		if (AFlareForgePlayerController* PlayerController = Cast<AFlareForgePlayerController>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller"));
			if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				//bUseDash = true;
				UE_LOG(LogTemp, Warning, TEXT("Dash Ability"));
				PlayerController->Dash();
				
				
				/*const FVector MoveDirection = Character->GetCharacterMovement()->GetLastInputVector();
		
				//double MaxMoveSpeed = GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
				FVector DashVector;
		
				if(MoveDirection.IsZero())
				{
					DashVector = FVector(DashSpeed * Character->GetActorForwardVector());
				}
				else
				{
					DashVector = FVector(DashSpeed * MoveDirection);
				}
			
				Character->LaunchCharacter(DashVector, false, false);
				//DashOnServer(Character, DashVector);
				//DashTimer = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) + DashCooldown;
			}
		}
	}*/


	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	
}

/*bool UDashAbility::DashAbilityIsActive() const
{
	return bUseDash;
}*/


/*void UDashAbility::DashOnServer_Implementation(ACharacter* Character, const FVector& DashVector) const
{
	Character->LaunchCharacter(DashVector, false, false);
}*/


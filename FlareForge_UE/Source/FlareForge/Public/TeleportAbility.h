// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TopDown/FlareForgeCharacter.h"
#include "TeleportAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UTeleportAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReceiveMouseData(FVector MouseLocation, FVector MouseDirection);
	void Server_ReceiveMouseData_Implementation(FVector MouseLocation, FVector MouseDirection);
	bool Server_ReceiveMouseData_Validate(FVector MouseLocation, FVector MouseDirection) { return true; }

private:
	
	UPROPERTY(EditAnywhere)
	float TeleportDistance = 1000.0f;
};

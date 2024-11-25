// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeflectShield.h"
#include "Abilities/GameplayAbility.h"
#include "DeflectAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UDeflectAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void DeflectAbility(const FVector SpawnDeflectShieldLocation, const FRotator CurrentRotation, FName PlayerName);
	
private:
	
	/*UPROPERTY(EditAnywhere)
	float ActiveTime = 5.0f;*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADeflectShield> ShieldBlueprint;

	UPROPERTY(VisibleAnywhere)
	ADeflectShield* Shield;

	/*UFUNCTION()
	void OnDeflectBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
	
};

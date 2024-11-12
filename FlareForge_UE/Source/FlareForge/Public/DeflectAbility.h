// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/BoxComponent.h"
#include "DeflectAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLAREFORGE_API UDeflectAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UDeflectAbility();
	
	//Override the ActivateAbility method
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* DeflectBox;
	
	UPROPERTY(EditAnywhere)
	float ActiveTime = 1.0f;

	/*UFUNCTION()
	void OnDeflectBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
	
};

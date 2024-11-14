// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LightBurst.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLAREFORGE_API ULightBurst : public USphereComponent
{
	GENERATED_BODY()
	
public:
	ULightBurst();

private:
	//UPROPERTY(EditAnywhere)
	//float SphereRadius;
};

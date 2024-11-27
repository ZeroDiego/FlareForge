// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingStrikeProjectile.generated.h"

UCLASS()
class FLAREFORGE_API AHomingStrikeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingStrikeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	USceneComponent* FindHomingStrikeTarget() const;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SpawningRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DeflectShieldRootComponent;

	UFUNCTION()
	void SetDeflectShieldRootComponent(USceneComponent* OtherRootComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LockOn(USceneComponent* TargetComponent, USceneComponent* PlayerSpawningRootComponent);

};

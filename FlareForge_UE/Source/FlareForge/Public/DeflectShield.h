// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DeflectShield.generated.h"

UCLASS()
class FLAREFORGE_API ADeflectShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeflectShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayerName(FName OtherName);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	void DeflectProjectile(AActor* Projectile, const FVector& HitLocation);

	FName PlayerName;
};

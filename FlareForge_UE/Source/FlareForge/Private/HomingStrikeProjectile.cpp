// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AHomingStrikeProjectile::AHomingStrikeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHomingStrikeProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (FindHomingStrikeTarget())
		LockOn(FindHomingStrikeTarget());
}

// Called every frame
void AHomingStrikeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USceneComponent* AHomingStrikeProjectile::FindHomingStrikeTarget() const
{
	AActor* HomingStrikeTarget = nullptr;
	double ClosestDistance = 5000.0f;
	
	TArray<AActor*> FoundTargets;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "HomingStrikeTarget", FoundTargets);

	for (AActor* FoundTarget : FoundTargets)
	{
		double VectorLength = UKismetMathLibrary::VSize(FoundTarget->GetActorLocation() - GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("VectorLength: %f"), VectorLength);
		UE_LOG(LogTemp, Warning, TEXT("ClosestDistance: %f"), ClosestDistance);
		
		if (VectorLength < ClosestDistance)
		{
			HomingStrikeTarget = FoundTarget;
			ClosestDistance = VectorLength;
		}

		UE_LOG(LogTemp, Warning, TEXT("VectorLength: %f"), VectorLength);
		UE_LOG(LogTemp, Warning, TEXT("ClosestDistance: %f"), ClosestDistance);
	}
	
	if (HomingStrikeTarget)
	{
		USceneComponent* TargetSceneComponent = HomingStrikeTarget->GetComponentByClass<USceneComponent>();
		return TargetSceneComponent;
	}

	return nullptr;
}

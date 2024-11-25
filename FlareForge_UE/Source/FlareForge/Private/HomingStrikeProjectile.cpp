// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeProjectile.h"

#include "EngineUtils.h"
#include "FlareForge/Character/MyPlayerCharacter.h"
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
}

// Called every frame
void AHomingStrikeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USceneComponent* AHomingStrikeProjectile::FindHomingStrikeTarget() const
{
	const AActor* HomingStrikeTarget = nullptr;
	double ClosestDistance = 5000.0f;
	
	TArray<AActor*> FoundTargets;

	for (TActorIterator<AMyPlayerCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr)
	{
		if (const double VectorLength = UKismetMathLibrary::VSize(CharacterItr->GetActorLocation() - GetActorLocation()); VectorLength < ClosestDistance)
		{
			HomingStrikeTarget = *CharacterItr;
			ClosestDistance = VectorLength;
		}
	}
	
	if (HomingStrikeTarget)
	{
		USceneComponent* TargetSceneComponent = HomingStrikeTarget->GetRootComponent();
		return TargetSceneComponent;
	}

	return nullptr;
}

void AHomingStrikeProjectile::SetDeflectShieldRootComponent(USceneComponent* OtherRootComponent)
{
	DeflectShieldRootComponent = OtherRootComponent;
}


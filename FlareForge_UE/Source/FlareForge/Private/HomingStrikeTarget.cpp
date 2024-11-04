// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingStrikeTarget.h"

// Sets default values
AHomingStrikeTarget::AHomingStrikeTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHomingStrikeTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomingStrikeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


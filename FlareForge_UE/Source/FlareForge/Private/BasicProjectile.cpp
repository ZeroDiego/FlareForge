// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectile.h"

// Sets default values
ABasicProjectile::ABasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "FightMeShield.h"

// Sets default values
AFightMeShield::AFightMeShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

}

// Called when the game starts or when spawned
void AFightMeShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFightMeShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


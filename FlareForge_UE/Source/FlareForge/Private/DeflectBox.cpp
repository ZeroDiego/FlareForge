// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectBox.h"

#include "DeflectAbility.h"

UDeflectBox::UDeflectBox()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDeflectBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDeflectBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




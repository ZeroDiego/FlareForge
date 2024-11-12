// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacter.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"
#include "MyPlayerState.h"


// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystemComponent();
	GiveDefaultAbilities();
	InitDefaultAttributes();
	//InitHUD();
}

void AMyPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystemComponent();
	InitDefaultAttributes();
	//InitHUD();
}

void AMyPlayerCharacter::InitAbilitySystemComponent()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	check(MyPlayerState);
	AbilitySystemComponent =  CastChecked<ULucasAbilitySystemComponent>(
		MyPlayerState->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);
	AttributeSet = MyPlayerState->GetAttributeSet();
}
/*
void AMyPlayerCharacter::InitHUD() const
{
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(AFlareForgeHUD* FabHUD = Cast<AFlareForgeHUD>(PlayerController->GetHUD()))
		{
			FabHUD->Init();
		}
	}
}*/

// Fill out your copyright notice in the Description page of Project Settings.


#include "FlareForgeHUD.h"
#include "FlareForge/Character/MyPlayerState.h"
#include "FlareForge/UI/AttributesWidget.h"

void AFlareForgeHUD::Init(){

	AttributeWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), AttributeWidgetClass);
	AttributeWidget->BindToAttributes(GetOwningPlayerController()->GetPlayerState<AMyPlayerState>());
	AttributeWidget->AddToViewport();
	
	}
// Fill out your copyright notice in the Description page of Project Settings.


#include "FlareForgeHUD.h"
#include "FlareForge/UI/AttributesWidget.h"

void AFlareForgeHUD::Init(){

	AttributeWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), AttributeWidgetClass);
	AttributeWidget->BindToAttributes();
	AttributeWidget->AddToViewport();
	
	}
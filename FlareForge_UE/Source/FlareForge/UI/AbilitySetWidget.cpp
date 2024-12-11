// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySetWidget.h"
#include "FlareForge/Character/MyPlayerState.h"

void UAbilitySetWidget::SetPlayerStateReference(AMyPlayerState* PlayerStateRef)
{
	PlayerStateReference = PlayerStateRef;
}

void UAbilitySetWidget::SetAbilityInPlayerState(int32 Index, TSubclassOf<UGameplayAbility> NewAbility)
{
	if (PlayerStateReference)
	{
		PlayerStateReference->SetAbilityAtIndex(Index, NewAbility);
	}
}

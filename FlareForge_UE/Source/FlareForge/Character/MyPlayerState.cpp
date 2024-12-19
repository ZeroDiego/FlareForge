// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"
#include "NetworkGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


AMyPlayerState::AMyPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<ULucasAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UMyCharacterAttributeSet>(TEXT("AttributeSet"));
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
			{
				IsMelee = NetworkGI->GetIsMelee();
			}
		}
	}
}

void AMyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (HasAuthority()) // Only initialize on the server
	{
		InitializeAbilities();
	}
}

void AMyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	AMyPlayerState* MyNewPlayerState = Cast<AMyPlayerState>(NewPlayerState);
	if (MyNewPlayerState)
	{
		MyNewPlayerState->SelectedAbilities = SelectedAbilities;
		MyNewPlayerState->UniquePlayerId = UniquePlayerId;

		// Reinitialize abilities after seamless travel
		MyNewPlayerState->InitializeAbilities();
		MyNewPlayerState->OnRep_SelectedAbilities();
	}
}

void AMyPlayerState::InitializeAbilities_Implementation()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	// Initialize ability actor info for the Ability System Component
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const FGameplayAbilitySpecHandle& Handle : SelectedAbilities)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->TryActivateAbility(Handle);
		}
	}
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMyCharacterAttributeSet* AMyPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AMyPlayerState::SetAbilityAtIndex_Implementation(int32 Index, TSubclassOf<UGameplayAbility> NewAbility)
{
	if (!HasAuthority() || !NewAbility) return;

	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Remove old ability if necessary
		FGameplayAbilitySpecHandle OldHandle = SelectedAbilities[Index];
		if (OldHandle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(OldHandle);
		}

		// Grant new ability and store its handle
		FGameplayAbilitySpec NewSpec(NewAbility, 1); // Level 1 by default
		FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(NewSpec);
		SelectedAbilities[Index] = NewHandle;
	}
	else
	{
		// Add new ability if index is out of bounds
		FGameplayAbilitySpec NewSpec(NewAbility, 1); // Level 1 by default
		FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(NewSpec);
		SelectedAbilities.Add(NewHandle);
	}

	OnRep_SelectedAbilities();
}
/*
TSubclassOf<UGameplayAbility> AMyPlayerState::GetAbilityAtIndex(int32 Index) const
{
	
	if (SelectedAbilities.IsValidIndex(Index))
	{
		return SelectedAbilities[Index];
	}
	return nullptr;
}*/

void AMyPlayerState::RemoveAbilityAtIndex(int32 Index)
{
	if (!HasAuthority()) return; // Ensure this runs on the server

	// Ensure that the index is within bounds
	if (SelectedAbilities.IsValidIndex(Index))
	{
		SelectedAbilities.RemoveAt(Index);
		OnRep_SelectedAbilities(); // Notify clients of the change
	}
}

void AMyPlayerState::SetIsMeleeTrue()
{
	if (HasAuthority()) // Ensure this runs on the server
	{
		IsMelee = true;

		if (const UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
				{
					NetworkGI->SetIsMelee(true);
				}
			}
		}
	}
}

void AMyPlayerState::SetIsMeleeFalse()
{
	if (HasAuthority()) // Ensure this runs on the server
	{
		IsMelee = false;

		if (const UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
				{
					NetworkGI->SetIsMelee(false);
				}
			}
		}
	}
}

void AMyPlayerState::SetUniquePlayerId_Implementation(const FString& NewId)
{
	UniquePlayerId = NewId;
}

FString AMyPlayerState::GetUniquePlayerId() const
{
	return UniquePlayerId;
}

void AMyPlayerState::OnRep_SelectedAbilities()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->ClearAllAbilities(); // Clear existing abilities

	for (const FGameplayAbilitySpecHandle& Handle : SelectedAbilities)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->TryActivateAbility(Handle);
		}
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this); // Reinitialize ASC info
	InitializeAbilities();
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate SelectedAbilities array
	DOREPLIFETIME(AMyPlayerState, SelectedAbilities);

	// Replicate UniquePlayerId to all clients
	DOREPLIFETIME(AMyPlayerState, UniquePlayerId);
}

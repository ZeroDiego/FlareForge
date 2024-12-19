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
	}
}

void AMyPlayerState::InitializeAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	// Initialize ability actor info for the Ability System Component
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : SelectedAbilities)
	{
		if (IsValid(AbilityClass))
		{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
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
		SelectedAbilities[Index] = NewAbility;
	}
	else
	{
		SelectedAbilities.Add(NewAbility);
	}

	OnRep_SelectedAbilities();
}

TSubclassOf<UGameplayAbility> AMyPlayerState::GetAbilityAtIndex(int32 Index) const
{
	
	if (SelectedAbilities.IsValidIndex(Index))
	{
		return SelectedAbilities[Index];
	}
	return nullptr;
}

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

void AMyPlayerState::TransferAbilitiesToAbilitySystemComponent_Implementation()
{
	if (!HasAuthority()) return; // Ensure this runs on the server

	if (AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : SelectedAbilities)
		{
			if (IsValid(AbilityClass))
			{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}

		// Reinitialize the Ability System Component with updated abilities
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ClearAllAbilities(); // Clear existing abilities

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : SelectedAbilities)
		{
			if (IsValid(AbilityClass))
			{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
        
		AbilitySystemComponent->InitAbilityActorInfo(this, this); // Reinitialize ASC info
	}
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate SelectedAbilities array
	DOREPLIFETIME(AMyPlayerState, SelectedAbilities);

	// Replicate UniquePlayerId to all clients
	DOREPLIFETIME(AMyPlayerState, UniquePlayerId);
}

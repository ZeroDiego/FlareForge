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

	// Ensure Ability System is initialized after all components are ready
	InitializeAbilities();
}

void AMyPlayerState::InitializeAbilities_Implementation() {
    if (!AbilitySystemComponent) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AbilitySystemComponent is null!"));
        }
        return;
    }

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative()) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Owner is not authoritative!"));
        }
        return;
    }

    if (const UWorld* World = GetWorld()) {
        if (UGameInstance* GameInstance = World->GetGameInstance()) {
            if (const UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance)) {
                // Use GetUniquePlayerIDFromState instead of GetUniquePlayerId
                const FString ThisUniquePlayerID = NetworkGI->GetUniquePlayerIDFromState(GetCustomDisplayName());

                const TArray<FGameplayAbilitySpec>& AbilitySpecs = NetworkGI->GetGameplayAbilitySpec(ThisUniquePlayerID);
                if (GEngine) {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Unique Player ID: %s"), *GetCustomDisplayName()));
                }

                for (const FGameplayAbilitySpec& GameplayAbilitySpec : AbilitySpecs) {
                    if (!GameplayAbilitySpec.Ability) {
                        if (GEngine) {
                            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Invalid Ability Spec: Ability is null!"));
                        }
                        continue;
                    }
                    // Assign the ability to the AbilitySystemComponent
                    AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);

                    // Debug message for successful assignment
                    if (GEngine) {
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Assigned Ability: %s"), *GameplayAbilitySpec.Ability->GetName()));
                    }
                }

                // Finalize initialization
                AbilitySystemComponent->InitAbilityActorInfo(this, this);
            }
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

void AMyPlayerState::SetAbilityAtIndex_Implementation(const int32 Index, const TSubclassOf<UGameplayAbility> NewAbility)
{
	if (!HasAuthority() || !NewAbility)
	{
		return;
	}

	// Ensure that the index is within bounds
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Replace the existing ability at this index
		SelectedAbilities[Index] = NewAbility;
	}
	else
	{
		// Resize the array to accommodate new index
		if (Index >= 0)
		{
			SelectedAbilities.SetNum(Index + 1);
			SelectedAbilities[Index] = NewAbility;
		}
	}
}

TSubclassOf<UGameplayAbility> AMyPlayerState::GetAbilityAtIndex(int32 Index) const
{
	// Check if the index is valid
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Return the ability at the specified index
		return SelectedAbilities[Index];
	}

	// Return nullptr or a default value if the index is invalid
	return nullptr;
}

void AMyPlayerState::RemoveAbilityAtIndex(int32 Index)
{
	// Check if AbilitySystemComponent is valid and we have authority
	if (!HasAuthority())
	{
		return;
	}

	// Ensure that the index is within bounds
	if (SelectedAbilities.IsValidIndex(Index))
	{
		// Remove the ability at the specified index
		SelectedAbilities.RemoveAt(Index);
	}
}

void AMyPlayerState::TransferAbilitiesToAbilitySystemComponent_Implementation()
{
    if (!HasAuthority()) return; // Ensure this runs on the server

    // Get all player states
    TArray<AActor*> PlayerStates;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyPlayerState::StaticClass(), PlayerStates);

    for (AActor* Actor : PlayerStates)
    {
        AMyPlayerState* PlayerState = Cast<AMyPlayerState>(Actor);
        if (PlayerState && PlayerState->AbilitySystemComponent)
        {
            for (int32 Index = 0; Index < PlayerState->SelectedAbilities.Num(); ++Index)
            {
                if (PlayerState->SelectedAbilities.IsValidIndex(Index))
                {
                    if (const TSubclassOf<UGameplayAbility> AbilityClass = PlayerState->SelectedAbilities[Index])
                    {
                        const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
                        PlayerState->AbilitySystemComponent->GiveAbility(AbilitySpec);

                        if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
                        {
                              if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
                            {
                              	//Add Player ID and Player State to GI
                              	NetworkGI->AddPlayerState(GetUniquePlayerId(), GetCustomDisplayName());
                              	// Use GetUniquePlayerIDFromState instead of GetUniquePlayerId
                              	const FString ThisUniquePlayerID = NetworkGI->GetUniquePlayerIDFromState(GetCustomDisplayName());
                              	
                                NetworkGI->SetGameplayAbilitySpecAtIndex(ThisUniquePlayerID, AbilitySpec, Index);
                                /*GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow,
                                    FString::Printf(TEXT("Transferred Ability: %s"), *AbilityClass->GetName()));*/
                            }
                        }
                    }
                }
            }

            if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
            {
                if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
                {
                	// Use GetUniquePlayerIDFromState instead of GetUniquePlayerId
                	const FString ThisUniquePlayerID = NetworkGI->GetUniquePlayerIDFromState(GetCustomDisplayName());
                    NetworkGI->SetSelectedAbilitiesForPlayer(ThisUniquePlayerID, PlayerState->GetSelectedAbilities());
                }
            }
        }
    }
}


const TArray<TSubclassOf<UGameplayAbility>>& AMyPlayerState::GetSelectedAbilities() const
{
	return SelectedAbilities;
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

FString AMyPlayerState::GetCustomDisplayName() const
{
	// Get the original display name
	const FString OriginalName = UKismetSystemLibrary::GetDisplayName(this);

	// Use a regular expression to extract numbers from the string
	FRegexPattern NumberPattern(TEXT("\\d+")); // Matches one or more digits
	FRegexMatcher Matcher(NumberPattern, OriginalName);

	if (Matcher.FindNext())
	{
		// Extract the matched number as a substring
		return Matcher.GetCaptureGroup(0);
	}

	// If no number is found, return an empty string or handle it as needed
	return FString();
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate SelectedAbilities array
	DOREPLIFETIME(AMyPlayerState, SelectedAbilities);

	// Replicate UniquePlayerId to all clients
	DOREPLIFETIME(AMyPlayerState, UniquePlayerId);
}

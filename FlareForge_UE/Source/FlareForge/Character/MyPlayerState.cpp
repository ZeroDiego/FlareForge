// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"
#include "NetworkGameInstance.h"


AMyPlayerState::AMyPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<ULucasAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UMyCharacterAttributeSet>(TEXT("AttributeSet"));
	bReplicates = true;
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if(const UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (const UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
			{
				if (const FGameplayAbilitySpec CurrentGameplayAbilitySpec = NetworkGI->GetGameplayAbilitySpec(); CurrentGameplayAbilitySpec.Ability)
				{
					AbilitySystemComponent->GiveAbility(CurrentGameplayAbilitySpec);
				
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, FString::Printf(TEXT("Granted Ability: %s"), *CurrentGameplayAbilitySpec.Ability->GetName()));
					}
				}
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

void AMyPlayerState::SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility)
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

void AMyPlayerState::RemoveAbility(TSubclassOf<UGameplayAbility> AbilityToRemove)
{
	// Check if AbilitySystemComponent is valid and we have authority
	if (!HasAuthority() || !AbilityToRemove)
	{
		return;
	}

	// Check if the ability exists in the SelectedAbilities array
	if (SelectedAbilities.Contains(AbilityToRemove))
	{
		// Remove the ability from the array
		SelectedAbilities.Remove(AbilityToRemove);
	}
}

void AMyPlayerState::TransferAbilitiesToASC()
{
	if (!AbilitySystemComponent) return;
	
	// Iterate over the first four indices: 0, 1, 2, and 3
	for (int32 Index = 0; Index < SelectedAbilities.Num(); ++Index)
	{
		if (SelectedAbilities.IsValidIndex(Index))
		{
			if (const TSubclassOf<UGameplayAbility> AbilityClass = SelectedAbilities[Index])
			{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
				
				if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
				{
					if (UNetworkGameInstance* NetworkGI = Cast<UNetworkGameInstance>(GameInstance))
					{
						NetworkGI->SetGameplayAbilitySpec(AbilitySpec);
						FGameplayAbilitySpec CurrentGameplayAbilitySpec = NetworkGI->GetGameplayAbilitySpec();
						
						if(GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, FString::Printf(TEXT("Granted Ability: %s"), *CurrentGameplayAbilitySpec.Ability->GetName()));
						}
					}
				}
			}
		}
	}
}

const TArray<TSubclassOf<UGameplayAbility>>& AMyPlayerState::GetSelectedAbilities() const
{
	return SelectedAbilities;
}
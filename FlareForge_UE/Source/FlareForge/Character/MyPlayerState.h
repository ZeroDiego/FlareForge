// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "MyPlayerState.generated.h"

class ULucasAbilitySystemComponent;
class UMyCharacterAttributeSet;
/**
 * 
 */
UCLASS()
class FLAREFORGE_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyPlayerState();
	
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void InitializeAbilities();
	
	virtual void PostInitializeComponents() override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

	const TArray<TSubclassOf<UGameplayAbility>>& GetSelectedAbilities() const;

	// Adds or sets an ability at a specific index in SelectedAbilities
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Ability")
	void SetAbilityAtIndex(int32 Index, TSubclassOf<UGameplayAbility> NewAbility);
	
	// Gets an ability from a specific index in SelectedAbilities
	UFUNCTION(BlueprintCallable, Category = "Ability")
	TSubclassOf<UGameplayAbility> GetAbilityAtIndex(int32 Index) const;
	
	// Removes an ability from a specific index in SelectedAbilities
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbilityAtIndex(int32 Index);

	// Transfers SelectedAbilities to the Ability System Component (ASC)
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Ability")
	void TransferAbilitiesToAbilitySystemComponent();
	
	//~IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface interface
	virtual UMyCharacterAttributeSet* GetAttributeSet() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool IsMelee;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetIsMeleeTrue();

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetIsMeleeFalse();

	// Set the unique player ID
	UFUNCTION(Server, Reliable)
	void SetUniquePlayerId(const FString& NewId);

	// Get the unique player ID
	UFUNCTION(BlueprintCallable)
	FString GetUniquePlayerId() const;
	// Get the custom player state display name
	UFUNCTION(BlueprintCallable, Category = "Player State")
	FString GetCustomDisplayName() const;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Customization")
	int32 SkinEquipped;
	
protected:	
	UPROPERTY()
	TObjectPtr<ULucasAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMyCharacterAttributeSet> AttributeSet;

	// Replicated list of selected abilities
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> SelectedAbilities;

	// Replicated list of all abilities
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> AllAbilities;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Replicated unique player ID
	UPROPERTY(Replicated, BlueprintReadOnly)
	FString UniquePlayerId;
};

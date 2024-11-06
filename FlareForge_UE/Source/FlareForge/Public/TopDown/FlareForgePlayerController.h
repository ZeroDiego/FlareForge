// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include <FlareForge/FlareForge.h>
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "FlareForgePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Define the FlareForgeAbilityInputID enum here in the header file
UENUM(BlueprintType)
enum class EFlareForgeAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AFlareForgePlayerController*, Character);

UCLASS()
class AFlareForgePlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFlareForgePlayerController(const class FObjectInitializer& ObjectInitializer);

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementVerticalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementHorizontalAction;
	
	/** Define MyAbilitySystemComponent **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	class UMyAbilitySystemComponent* MyAbilitySystemComponent;

	// Array to store default abilities to be granted to the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	//return AbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintAssignable, Category = "Character")
    virtual bool IsAlive() const;

	UFUNCTION(BlueprintAssignable, Category = "Character")
	virtual int32 GetAbilityLevel(FlareForgeAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilites();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void FinishDying();

	//Get attributes
	UFUNCTION(BlueprintCallable, Category = "Character_Attributes")
	float GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character_Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character_Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character_Attributes")
	float GetPower() const;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	// movement functions
	void MovementVertical(const FInputActionValue& Value);
	void MovementHorizontal(const FInputActionValue& Value);

	// rotate character with mouse
	void RotatePlayerTowardsMouse();

	UFUNCTION(Server, Reliable)
	void RotatePlayerOnServer(const FRotator PlayerRotation);

	TWeakObjectPtr<class UMyAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UMyCharacterAttributeSet> AttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<class UCharacterGameplayAbility> CharacterAbilities>
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TArray<TSubclassOf<class UGameplayEffect> StartupEffects>;

	virtual void AddCharacterAbilites();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

    //Set Health on spawn
	virtual void SetHealth(float Health);

	
private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};



// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

UCLASS()
class AFlareForgePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFlareForgePlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, Blueprintable)
	float DashSpeed = 100.0f;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** GamepadMappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* GamepadMappingContext;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ControllerRotationAction;
	
	/** Define MyAbilitySystemComponent **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	class UMyAbilitySystemComponent* MyAbilitySystemComponent;

	// Array to store default abilities to be granted to the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	// Dash functions
	void Dash();

	UFUNCTION(Server, Reliable)
	void DashOnServer(const FVector& DashVector) const;

	UFUNCTION(Server, Reliable)
	void InitAbilitySystem();

	static int32 InstanceCounter;

	// Instance-specific ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 InstanceID;
	
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

	void ApplyMovement();
	
	// rotate character with mouse
	void RotatePlayerTowardsMouse();

	// rotate character with joystick
	void RotatePlayerTowardsJoystick(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void RotatePlayerOnServer(const FRotator PlayerRotation);
	

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	FVector2D InputVector = FVector2D::ZeroVector;
	
	float DashTimer = 0.0f;
	
};



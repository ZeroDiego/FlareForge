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

/*// Define the FlareForgeAbilityInputID enum here in the header file
UENUM(BlueprintType)
enum class EFlareForgeAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel")
};
*/
UCLASS()
class AFlareForgePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFlareForgePlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCooldown = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AnimationVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpAlphaValueForward = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpAlphaValueBackward = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpAlphaValueRight = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpAlphaValueLeft = 0.05f;


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

	// Dash functions
	void Dash();

	UFUNCTION(Server, Reliable)
	void DashOnServer(const FVector& DashVector) const;

	UFUNCTION(BlueprintCallable)
	FVector GetAnimationVelocity();

	// Keeps track of the number of instances that
	// have been created of this class thus far
	//static int32 InstanceCounter;

	// Instance ID that increments after a new instance is created
	// So the first instance is 0 and the one after is 1 and so on
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//int32 InstanceID = 0;
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;
	
	// To add mapping context
	virtual void BeginPlay();
	
	virtual void ClientRestart_Implementation(APawn* NewPawn) override;

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

	float CurrentLerpValueForward = 0.0f;
	float CurrentLerpValueRight = 0.0f;
	
};
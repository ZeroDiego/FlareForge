// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDown/FlareForgeCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MyAbilitySystemComponent.h"
#include "TeleportAbility.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFlareForgePlayerController::AFlareForgePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	// Initialize MyAbilitySystemComponent
	MyAbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("MyAbilitySystemComponent"));
}

void AFlareForgePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Ensure MyAbilitySystemComponent is valid
	if (MyAbilitySystemComponent)
	{
		// Grant each ability in the DefaultAbilities array
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
		{
			if (Ability)
			{
				MyAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(EFlareForgeAbilityInputID::Confirm), this));
			}
		}
	}
}

void AFlareForgePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFlareForgePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFlareForgePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFlareForgePlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFlareForgePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFlareForgePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFlareForgePlayerController::OnTouchReleased);

		// Setup Movement
		/*EnhancedInputComponent->BindAction(HorizontalMovementAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::HorizontalMovement);
		EnhancedInputComponent->BindAction(VerticalMovementAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::VerticalMovement);*/
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::Movement);

		// Ensure MyAbilitySystemComponent is valid and bind it to the input
		if (MyAbilitySystemComponent && InputComponent)
		{
			MyAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(
				"Confirm",
				"Cancel",
				FTopLevelAssetPath(TEXT("/Script/FlareForge"), TEXT("EFlareForgeAbilityInputID")),
				static_cast<int32>(EFlareForgeAbilityInputID::Confirm),
				static_cast<int32>(EFlareForgeAbilityInputID::Cancel)));
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

/*void AFlareForgePlayerController::HorizontalMovement(const FInputActionValue& Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	if(GetCharacter())
	{
		GetCharacter()->AddMovementInput(Direction, Value);
	}
	UE_LOG(LogTemp, Warning, TEXT("Horizontal"));
	FVector2D MovementVector = Value.Get<FVector2D>();
	if(GetPawn())
	{
		GetPawn()->AddMovementInput(GetCharacter()->GetActorForwardVector(), MovementVector.Y);
	}
}

void AFlareForgePlayerController::VerticalMovement(const FInputActionValue& Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if(GetCharacter())
	{
		GetCharacter()->AddMovementInput(Direction, Value);
	}
}*/

void AFlareForgePlayerController::Movement(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));

	GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), 1);
}



void AFlareForgePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AFlareForgePlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AFlareForgePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AFlareForgePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AFlareForgePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgePlayerController.h"
#include "GameFramework/Pawn.h"
#include "TopDown/FlareForgeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TeleportAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Initialize the static counter to 0
int32 AFlareForgePlayerController::InstanceCounter = 0;

AFlareForgePlayerController::AFlareForgePlayerController()
{
	// Assign the current value of InstanceCounter to InstanceID
	InstanceID = InstanceCounter;
	
	// Increment the static counter for the next instance
	InstanceCounter++;
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

}

void AFlareForgePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

void AFlareForgePlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())->HasMappingContext(GamepadMappingContext))
	{
		RotatePlayerTowardsMouse();	
	}
}


void AFlareForgePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	// Add Input Mapping Context
	if(InstanceID == 0)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			/*// Setup mouse input events
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFlareForgePlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFlareForgePlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFlareForgePlayerController::OnSetDestinationReleased);
	
			// Setup touch input events
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFlareForgePlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::OnTouchTriggered);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFlareForgePlayerController::OnTouchReleased);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFlareForgePlayerController::OnTouchReleased);*/

			// Setup Movement
			EnhancedInputComponent->BindAction(MovementHorizontalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementHorizontal);
			EnhancedInputComponent->BindAction(MovementVerticalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementVertical);

			// Dash Setup
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::Dash);
			
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
	else if (InstanceID == 1)
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GamepadMappingContext, 0);
		}
		
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Setup Movement
			EnhancedInputComponent->BindAction(MovementHorizontalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementHorizontal);
			EnhancedInputComponent->BindAction(MovementVerticalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementVertical);

			// Dash Setup
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::Dash);

			// Controller Rotation Setup
			EnhancedInputComponent->BindAction(ControllerRotationAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::RotatePlayerTowardsJoystick);
			
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}

// Get Y value of vector
void AFlareForgePlayerController::MovementHorizontal(const FInputActionValue& Value)
{
	InputVector.Y = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Get X value of vector
void AFlareForgePlayerController::MovementVertical(const FInputActionValue& Value)
{
	InputVector.X = Value.Get<FVector2D>().X;
	ApplyMovement();
}

// Apply both X and Y values to and place the vector to player character
void AFlareForgePlayerController::ApplyMovement()
{
	//Normalize input so diagonal input is not hypotenuse from input
	FVector2D NormalizedInput = InputVector.GetSafeNormal();

	// Get rotation
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// Y input
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (GetCharacter())
	{
		GetCharacter()->AddMovementInput(RightDirection, NormalizedInput.Y);
	}

    // X input
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	if (GetCharacter())
	{
		GetCharacter()->AddMovementInput(ForwardDirection, NormalizedInput.X);
	}

    // reset vector at the end
	InputVector = FVector2D::ZeroVector;
}


void AFlareForgePlayerController::Dash()
{
	if(DashTimer < UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()))
	{
		if (GetCharacter())
		{
			const FVector MoveDirection = GetCharacter()->GetCharacterMovement()->GetLastInputVector();
		
			//double MaxMoveSpeed = GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
			FVector DashVector;
		
			if(MoveDirection.IsZero())
			{
				DashVector = FVector(DashSpeed * GetCharacter()->GetActorForwardVector());
			}
			else
			{
				DashVector = FVector(DashSpeed * MoveDirection);
			}
			
			GetCharacter()->LaunchCharacter(DashVector, false, false);
			DashOnServer(DashVector);
			DashTimer = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) + 3;
		}
	}
	
}

void AFlareForgePlayerController::DashOnServer_Implementation(const FVector& DashVector) const
{
	GetCharacter()->LaunchCharacter(DashVector, false, false);
}

void AFlareForgePlayerController::RotatePlayerTowardsMouse()
{
    if (FVector MouseLocation, MouseDirection; this->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
    {
        // Get local reference to the controller's character
        ACharacter* CurrentChar = this->GetCharacter();
        if (!CurrentChar) return;

        FVector CharacterLocation = CurrentChar->GetActorLocation();

        // Ray trace to find the intersection with the plane the character is on
        FVector Start = MouseLocation;
        FVector End = MouseLocation + MouseDirection * 10000.0f;  // Extend the ray

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(CurrentChar);
        
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
        {
            // Ensure the hit location is on the same plane as the character
            FVector HitLocation = HitResult.Location;
            FVector CharacterPlaneLocation = FVector(HitLocation.X, HitLocation.Y, CharacterLocation.Z);

            FVector Direction = CharacterPlaneLocation - CharacterLocation;

            // Calculate the new rotation
            FRotator TargetRotation = Direction.Rotation();
            FRotator CharRotation = CurrentChar->GetActorRotation();
            FRotator NewRot = FRotator(CharRotation.Pitch, TargetRotation.Yaw, CharRotation.Roll);

            // Set the new rotation
            CurrentChar->SetActorRotation(NewRot);
        	RotatePlayerOnServer(NewRot);
        }
    }
}

void AFlareForgePlayerController::RotatePlayerTowardsJoystick(const FInputActionValue& Value)
{
	ACharacter* CurrentChar = this->GetCharacter();
	if (!CurrentChar) return;

	const float XValue = Value.Get<FVector2D>().X;
	const float YValue = Value.Get<FVector2D>().Y;

	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(FVector(0.0, 0.0, 0.0), FVector(XValue, YValue, 0.0));
	
	CurrentChar->SetActorRotation(NewRot);
}


void AFlareForgePlayerController::RotatePlayerOnServer_Implementation(const FRotator PlayerRotation)
{
	this->GetCharacter()->SetActorRotation(PlayerRotation);
}

/*
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
*/
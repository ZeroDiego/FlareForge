// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgePlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "TopDown/FlareForgeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TeleportAbility.h"
#include "FlareForge/UI/FlareForgeHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Initialize the static counter to 0
//int32 AFlareForgePlayerController::InstanceCounter = 0;

AFlareForgePlayerController::AFlareForgePlayerController()
{
	// Assign the current value of InstanceCounter to InstanceID
	//InstanceID = InstanceCounter;
	
	// Increment the static counter for the next instance
	//InstanceCounter++;
	
	bShowMouseCursor = true;
	bShouldRotateTowardsMouse = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	bReplicates = true;
	bCanRemoveLoadingScreen = false;
	
}

void AFlareForgePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

void AFlareForgePlayerController::ClientRestart_Implementation(APawn* NewPawn)
{
	Super::ClientRestart_Implementation(NewPawn);

	if (GetHUD())
	{
		if (AFlareForgeHUD* FlareForgeHUD = Cast<AFlareForgeHUD>(GetHUD()))
		{
			FlareForgeHUD->Init();
			//RemoveLoadingScreenFromPlayer();
			/*if(IsLocalController())
			{
				ServerHandleClientRestart();
			}*/
			bCanRemoveLoadingScreen = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("HUD is not of type FlareForgeHUD in ClientRestart!"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("HUD is NULL in ClientRestart!"));
	}
}

/*void AFlareForgePlayerController::ServerHandleClientRestart_Implementation()
{
	//UE_LOG(LogTemp, Display, TEXT("ServerHandleClientRestart_Implementation"));
	RemoveLoadingScreenFromServer();
}*/


void AFlareForgePlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//if (!ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())->HasMappingContext(GamepadMappingContext))
	//{
	RotatePlayerTowardsMouse();
	//GetAnimationVelocity();
	//}
}


void AFlareForgePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
	// Add Input Mapping Context depending on which instance
	// because instance 0 is player 0 who plays on mouse
	// and instance 1 is player 1 who plays on gamepad
	//if(InstanceID == 0)
	//{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
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
	//}
	//else if (InstanceID == 1)
	/*{
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
	}*/
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

FVector AFlareForgePlayerController::GetAnimationVelocity()
{
	FVector Velocity = GetCharacter()->GetVelocity();
	
	FVector NormalizedVelocity = Velocity.GetSafeNormal();
	
	FVector CurrentForwardVector = GetCharacter()->GetActorForwardVector();
	FVector CurrentRightVector = GetCharacter()->GetActorRightVector();
	
	float ForwardVelocity = FVector::DotProduct(NormalizedVelocity, CurrentForwardVector);
	float RightVelocity = FVector::DotProduct(NormalizedVelocity, CurrentRightVector);

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Lerp with different values if character is moving forward or backwards
	if(ForwardVelocity >= 0.0f)
	{
		CurrentLerpValueForward = FMath::Lerp(CurrentLerpValueForward, ForwardVelocity, LerpAlphaValueForward * DeltaTime);
	}
	else if (ForwardVelocity < 0.0f)
	{
		CurrentLerpValueForward = FMath::Lerp(CurrentLerpValueForward, ForwardVelocity, LerpAlphaValueBackward * DeltaTime);
	}

	// Lerp with different values if character is moving left or right
	if(RightVelocity >= 0.0f)
	{
		CurrentLerpValueRight = FMath::Lerp(CurrentLerpValueRight, RightVelocity, LerpAlphaValueRight * DeltaTime);
	}
	else if(RightVelocity < 0.0f)
	{
		CurrentLerpValueRight = FMath::Lerp(CurrentLerpValueRight, RightVelocity, LerpAlphaValueLeft * DeltaTime);
	}
	//CurrentLerpValueForward = FMath::Lerp(CurrentLerpValueForward, ForwardVelocity, LerpAlphaValueForward * DeltaTime);
	//CurrentLerpValueRight = FMath::Lerp(CurrentLerpValueRight, RightVelocity, LerpAlphaValueRight * DeltaTime);
	
	AnimationVelocity = FVector(CurrentLerpValueForward, CurrentLerpValueRight, 0.0f);
	
	return AnimationVelocity;
}

void AFlareForgePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlareForgePlayerController, bShouldRotateTowardsMouse);
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
				// GetSafeNormal makes diagonal movement the same as vertical and horizontal
				DashVector = FVector(DashSpeed * MoveDirection.GetSafeNormal());
			}
			
			//GetCharacter()->LaunchCharacter(DashVector, false, false);
			PlayDashAnimation();
			DashOnServer(DashVector);
			DashTimer = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) + DashCooldown;
		}
	}
	
}

void AFlareForgePlayerController::DashOnServer_Implementation(const FVector& DashVector) const
{
	GetCharacter()->LaunchCharacter(DashVector, false, false);
	//PlayDashAnimation();
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
        
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, Params))
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
        	if(bShouldRotateTowardsMouse)
        	{
        		CurrentChar->SetActorRotation(NewRot);
        		RotatePlayerOnServer(NewRot);	
        	}
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
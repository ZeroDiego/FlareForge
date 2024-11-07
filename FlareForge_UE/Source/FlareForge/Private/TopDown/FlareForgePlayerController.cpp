// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDown/FlareForgeCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MyAbilitySystemComponent.h"
#include "TeleportAbility.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacterAttributeSet.h"
#include "CharacterGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFlareForgePlayerController::AFlareForgePlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	// Initialize MyAbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));


		

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

    bAlwaysRelevant = true;
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));

}

UAbilitySystemComponent* AFlareForgePlayerController::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AFlareForgePlayerController::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AFlareForgePlayerController::GetAbilityLevel(EFlareForgeAbilityID AbilityID) const
{
	return 1;
}

void AFlareForgePlayerController::RemoveCharacterAbilites()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitesGiven)
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesRemove;
	for(const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilites())
	{
		if((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
 }

float AFlareForgePlayerController::GetCharacterLevel() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetLevel();
	}
	return 0.0f;
}

float AFlareForgePlayerController::GetHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AFlareForgePlayerController::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AFlareForgePlayerController::GetPower() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetPower();
	}
	return 0.0f;
}
/*
void AFlareForgePlayerController::Die()
{
	RemoveCharacterAbilites();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}else
	{
		FinishDying();
	}
	
}
*/

void AFlareForgePlayerController::FinishDying()
{
	Destroy();
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

void AFlareForgePlayerController::AddCharacterAbilites()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for(TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AFlareForgePlayerController::InitializeAttributes()
{
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AFlareForgePlayerController::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if(NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AFlareForgePlayerController::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AFlareForgePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	RotatePlayerTowardsMouse();
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
		EnhancedInputComponent->BindAction(MovementHorizontalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementHorizontal);
		EnhancedInputComponent->BindAction(MovementVerticalAction, ETriggerEvent::Triggered, this, &AFlareForgePlayerController::MovementVertical);

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



void AFlareForgePlayerController::MovementHorizontal(const FInputActionValue& Value)
{
	
	//GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value.Get<FVector2D>().X);
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	GetCharacter()->AddMovementInput(Direction, Value.Get<FVector2D>().X);
}

void AFlareForgePlayerController::MovementVertical(const FInputActionValue& Value)
{
	
	//GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value.Get<FVector2D>().X);
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	GetCharacter()->AddMovementInput(Direction, Value.Get<FVector2D>().X);
}

void AFlareForgePlayerController::RotatePlayerTowardsMouse()
{
    FVector MouseLocation, MouseDirection;
    if (this->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
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
            //DrawDebugLine(GetWorld(), Start, End, FColor(255,0,0),false,1);
            CurrentChar->SetActorRotation(NewRot);
        	RotatePlayerOnServer(NewRot);
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("No hit detected"));
        }
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("Deprojection failed"));
    }
}

void AFlareForgePlayerController::RotatePlayerOnServer_Implementation(const FRotator PlayerRotation)
{
	this->GetCharacter()->SetActorRotation(PlayerRotation);
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

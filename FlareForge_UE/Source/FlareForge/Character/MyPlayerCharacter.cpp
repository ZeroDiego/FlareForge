// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacter.h"
#include "LucasAbilitySystemComponent.h"
#include "MyCharacterAttributeSet.h"
#include "MyPlayerState.h"
#include "FlareForge/UI/FlareForgeHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"


// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bReplicates = true;
}

void AMyPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	    
	InitAbilitySystemComponent();
	InitDefaultAttributes();
	if(HasAuthority())
		InitHUD();
	/*if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Possess"));*/
}

void AMyPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystemComponent();
	InitDefaultAttributes();
	APlayerController* PC = Cast<APlayerController>(GetController());
	/*if (PC)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("PlayerController is valid"));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("PlayerController is NULL on the client."));
	}

	AMyPlayerState* MyPlayerState = PC ? PC->GetPlayerState<AMyPlayerState>() : nullptr;
	if (MyPlayerState)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("PlayerState is valid"));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("PlayerState is NULL on the client."));
	}*/
	
	InitHUD();
}

void AMyPlayerCharacter::InitAbilitySystemComponent()
{
	if (AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>())
	{
		AbilitySystemComponent = Cast<ULucasAbilitySystemComponent>(MyPlayerState->GetAbilitySystemComponent());
		if (!AbilitySystemComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast AbilitySystemComponent to ULucasAbilitySystemComponent"));
			return;
		}

		AttributeSet = MyPlayerState->GetAttributeSet();

		// Initialize ASC with character as avatar
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);
		}
	}
}

void AMyPlayerCharacter::InitHUD() const
{
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(AFlareForgeHUD* FlareForgeHUD = Cast<AFlareForgeHUD>(PlayerController->GetHUD()))
		{
			FlareForgeHUD->Init();
		}
	}
}

void AMyPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
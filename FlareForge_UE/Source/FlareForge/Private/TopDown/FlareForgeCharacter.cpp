// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown/FlareForgeCharacter.h"

#include "GameplayTagContainer.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "CharacterPlayerState.h"
#include "CharacterGameplayAbility.h"
#include "MyCharacterAttributeSet.h"
#include "LucasAbilitySystemComponent.h"

AFlareForgeCharacter::AFlareForgeCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	ULucasAbilitySystemComponent* LucasAbilitySystemComponent = CreateDefaultSubobject<ULucasAbilitySystemComponent>(TEXT("LucasAbilitySystemComponent"));
	
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

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void AFlareForgeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UAbilitySystemComponent* AFlareForgeCharacter::GetAbilitySystemComponent() const
{
	return LucasAbilitySystemComponent;
}

bool AFlareForgeCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AFlareForgeCharacter::GetAbilityLevel(EFlareForgeAbilityID AbilityID) const
{
	return 1;
}

void AFlareForgeCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !MyAbilitySystemComponent.IsValid() || !MyAbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& Spec : MyAbilitySystemComponent->GetActivatableAbilities())
	{
		if((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
 }

float AFlareForgeCharacter::GetCharacterLevel() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetLevel();
	}
	return 0.0f;
}

float AFlareForgeCharacter::GetHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AFlareForgeCharacter::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AFlareForgeCharacter::GetPower() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetPower();
	}
	return 0.0f;
}

void AFlareForgeCharacter::Die()
{
	RemoveCharacterAbilities();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);
	if(MyAbilitySystemComponent.IsValid())
	{
		MyAbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = MyAbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		MyAbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}else
	{
		FinishDying();
	}
}

void AFlareForgeCharacter::FinishDying()
{
	Destroy();
}

void AFlareForgeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (IsValid(LucasAbilitySystemComponent))
	{
		// Initialize any necessary mappings or abilities here
		// Example: Binding default abilities to input actions
		LucasAbilitySystemComponent->SetInputBinding(MovementHorizontalAction, MovementHorizontalHandle);
	}
}

void AFlareForgeCharacter::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !MyAbilitySystemComponent.IsValid() || MyAbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for(TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		MyAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	MyAbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AFlareForgeCharacter::InitializeAttributes()
{
	if(!MyAbilitySystemComponent.IsValid())
	{
		return;
	}

	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
	}

	FGameplayEffectContextHandle EffectContext = MyAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = MyAbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = MyAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), MyAbilitySystemComponent.Get());
	}
}

void AFlareForgeCharacter::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority || !MyAbilitySystemComponent.IsValid() || MyAbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = MyAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = MyAbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if(NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = MyAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), MyAbilitySystemComponent.Get());
		}
	}
	MyAbilitySystemComponent->StartupEffectsApplied = true;
}

void AFlareForgeCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}
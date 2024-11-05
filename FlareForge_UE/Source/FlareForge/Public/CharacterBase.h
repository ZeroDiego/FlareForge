// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "TopDown/FlareForgeCharacter.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"
/**
 * 
 */

UCLASS()
class FLAREFORGE_API ACharacterBase : public AFlareForgeCharacter, public IAbilitySystemInterface
{
	
	GENERATED_BODY()

public:
	//Set default values for this character's properties
	ACharacterBase();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
    const class UMyCharacterAttributeSet* CharacterAttributeSet;

	virtual void BeginPlay() override;
	
};
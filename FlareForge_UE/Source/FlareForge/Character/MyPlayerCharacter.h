// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "FlareForge/UI/FlareForgeHUD.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class FLAREFORGE_API AMyPlayerCharacter : public AMyCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bHello;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bDead;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float CastTime;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFlareForgeHUD> NorseForgeHUD;
};

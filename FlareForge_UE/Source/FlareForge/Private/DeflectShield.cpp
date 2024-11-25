// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectShield.h"

#include "HomingStrikeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADeflectShield::ADeflectShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	BoxComponent->SetCollisionProfileName(TEXT("ProjectileTrigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeflectShield::OnBoxOverlap);

}

// Called when the game starts or when spawned
void ADeflectShield::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADeflectShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeflectShield::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	//UE_LOG(LogTemp, Warning, TEXT("trace channel: %d"), OtherActor->GetRootComponent()->GetCollisionObjectType());
	
	if(OtherActor->GetRootComponent()->GetCollisionObjectType() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("point: %s"), *SweepResult.ImpactPoint.ToString());
		DeflectProjectile(OtherActor, SweepResult.ImpactPoint);
	}
	
}

void ADeflectShield::DeflectProjectile(AActor* Projectile, const FVector& HitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Deflect projectile"));

	FVector ProjectileVelocity = Projectile->GetVelocity();
	if(ProjectileVelocity.IsNearlyZero())
	{
		return;
	}

	FVector SpawnLocation = Projectile->GetActorLocation();
	if(!HitLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("not nearly zero"));
		SpawnLocation = HitLocation;
	}
	
	Projectile->Destroy();

	FVector ReflectedVelocity = -ProjectileVelocity;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AHomingStrikeProjectile* NewProjectile = GetWorld()->SpawnActor<AHomingStrikeProjectile>(Projectile->GetClass(), SpawnLocation, ReflectedVelocity.Rotation(), SpawnParams);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Projectile"));

	
	if(NewProjectile)
	{
		NewProjectile->SetDeflectShieldRootComponent(NewProjectile->FindComponentByClass<USceneComponent>());
		FName ProjectileName = "DeflectProjectile";
		//FName FullName = FName::;
		//NewProjectile->Tags.Add();
		UProjectileMovementComponent* ProjectileMovement = NewProjectile->FindComponentByClass<UProjectileMovementComponent>();
		if(ProjectileMovement)
		{
			UE_LOG(LogTemp, Warning, TEXT("hitlocation: %s"), *HitLocation.ToString());
			ProjectileMovement->Velocity = ReflectedVelocity;
		}
	}
}

void ADeflectShield::SetPlayerName(FName OtherName)
{
	PlayerName = OtherName;
}






#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/HumanType.h"
#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "CRope.generated.h"

UCLASS()
class MYPROJECT_06_API ACRope : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly)
		UCableComponent* CableComponent;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent;
public:
	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		class UStaticMesh* HookMesh;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		float ThrowSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		float ThrowDistance = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		float SpinSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		float RopeThickness = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		float PenetrationOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		class UMaterialInterface* RopeMaterial;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		class USoundBase* RopeThrowSound;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		class USoundBase* RopePullSound;

	UPROPERTY(EditAnywhere, Category = "Hook Settings")
		FHitData HitData;

public:	
	ACRope();

public:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void InterpToPoint();
	void RotateHook();

	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	AHumanType* Owner;

	float RotationDirection = 0.0f;
	FVector HookEndTrace = FVector::ZeroVector;
	FVector MovingActorOffset = FVector::ZeroVector;
	FVector HookSocketLocation = FVector::ZeroVector;
	bool Active = false;
	bool IsEnter = false;

	FHitResult* HitResult;

	
};



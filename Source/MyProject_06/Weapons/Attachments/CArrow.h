#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/HumanType.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);


UCLASS()
class MYPROJECT_06_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpanAfterCollision = 20;

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;


	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent;


	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> CameraShake;

	UPROPERTY(EditAnywhere)
		FHitData SkillHitData;

public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
	ACArrow();

protected:
	UFUNCTION(BlueprintNativeEvent)
		void BeginPlay() override;
	virtual void BeginPlay_Implementation();


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Shoot(const FVector& InForward);

	void RangeAttack();

private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:
	FProjectileHit OnHit;
	FProjectileEndPlay OnEndPlay;

	int32 ActionIndex = 0;
	float RangeRadius = 500.0f;

	bool IsSkillShoot = false;
private:
	TArray<AActor*> Ignores;
	FVector InDirection;

	TArray<FHitResult> HitResults;
	TArray<AHumanType*> Victims;
};


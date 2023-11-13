#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Weapons/CWeaponStructures.h"
#include "Character/HumanType.h"
#include "CANS_RangeCollision.generated.h"

UCLASS()
class MYPROJECT_06_API UCANS_RangeCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Radius = 100.0f;

	UPROPERTY(EditAnywhere)
		FVector Margin = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		bool HitLoop = false;

	UPROPERTY(EditAnywhere)
		float HitTimer = -1.0f;


	UPROPERTY(EditAnywhere)
		FHitData HitData;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;


private:
	TArray<AHumanType*> Hitted;
	AHumanType* Owner;

	FVector StartLocation = FVector::ZeroVector;


	float DeltaTime = 0.0f;
};

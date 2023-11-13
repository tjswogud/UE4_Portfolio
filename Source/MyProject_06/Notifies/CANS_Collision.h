#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeaponStructures.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_Collision.generated.h"

UCLASS()
class MYPROJECT_06_API UCANS_Collision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FHitData HitData;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

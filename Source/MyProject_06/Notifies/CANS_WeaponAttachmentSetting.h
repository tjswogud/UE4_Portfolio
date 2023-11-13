#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_WeaponAttachmentSetting.generated.h"

UCLASS()
class MYPROJECT_06_API UCANS_WeaponAttachmentSetting : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		bool bWeaponVisible = true;

	UPROPERTY(EditAnywhere)
		bool bWeaponUpsideDown = false;


private:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

private:
	class AHumanType* Owner;

	USceneComponent* WeaponMesh;
};

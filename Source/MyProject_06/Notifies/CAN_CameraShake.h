#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraShake.h"
#include "CAN_CameraShake.generated.h"

UCLASS()
class MYPROJECT_06_API UCAN_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:



	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> CameraShake;


public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:

};

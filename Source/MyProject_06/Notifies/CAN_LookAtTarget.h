#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Character/HumanType.h"
#include "CAN_LookAtTarget.generated.h"

UCLASS()
class MYPROJECT_06_API UCAN_LookAtTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		bool bUsePitchAndRoll = false;
	


private:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


	
private:
	FName TargetName;

	AHumanType* Target;
};

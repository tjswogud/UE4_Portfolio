#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Component/CStateComponent.h"
#include "CAN_BeginState.generated.h"


UCLASS()
class MYPROJECT_06_API UCAN_BeginState : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Type")
		EStateType StateType;

	UPROPERTY(EditAnywhere, Category = "Ground")
		bool IsFront;

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


};

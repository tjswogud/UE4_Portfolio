#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Component/CMovementComponent.h"
#include "CAN_SetMoveComp.generated.h"

UCLASS()
class MYPROJECT_06_API UCAN_SetMoveComp : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "EnableFixCamera")
		bool FixCamera = false;

	UPROPERTY(EditAnywhere, Category = "EnableControlRotaion")
		bool EnableControlRotation = true;

	UPROPERTY(EditAnywhere, Category = "CanMove")
		bool CanMove = false;

	UPROPERTY(EditAnywhere, Category = "SetSpeed")
		ESpeedType MoveSpeed = ESpeedType::Max;

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	
private:
	
};

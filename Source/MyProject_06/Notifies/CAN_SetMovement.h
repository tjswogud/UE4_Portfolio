#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Component/CMovementComponent.h"
#include "CAN_SetMovement.generated.h"

UCLASS()
class MYPROJECT_06_API UCAN_SetMovement : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "EnableFixCamera")
		bool FixCamera;

	UPROPERTY(EditAnywhere, Category = "EnableControlRotaion")
		bool EnableControlRotation;

	UPROPERTY(EditAnywhere, Category = "CanMove")
		bool CanMove;

	UPROPERTY(EditAnywhere, Category = "SetSpeed")
		ESpeedType MoveSpeed = ESpeedType::Max;

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	
};

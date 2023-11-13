#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAN_Move.generated.h"


UENUM(BlueprintType)
enum class EMoveType : uint8
{
	Velocity = 0, Launch, Force, Impulse
};

UENUM(BlueprintType)
enum class EPowerDirection : uint8
{
	Forward = 0, Backward, Left, Right, Up, Down, ForwardDown, ForwardUp, Random
};

UCLASS()
class MYPROJECT_06_API UCAN_Move : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EMoveType MoveType = EMoveType::Launch;

	UPROPERTY(EditAnywhere)
		EPowerDirection MoveDirection = EPowerDirection::Forward;

	UPROPERTY(EditAnywhere)
		float Power = 1000.0f;

	UPROPERTY(EditAnywhere)
		bool IsOverride = false;

	UPROPERTY(EditAnywhere)
		bool bSetVelocityZero = false;

	UPROPERTY(EditAnywhere)
		float ForwardUpRatio = 1.0f;


private:
	FVector DirectVector = FVector::ZeroVector;

private:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
private:
	void GetDirectionVector(EPowerDirection Direction);


private:
	ACharacter* Owner = nullptr;

};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Weapons/CWeaponAsset.h"
#include "CANS_SkillComboAction.generated.h"

UENUM()
enum class EComboCondition
{
	Collsion = 0, Command, Random
};


UCLASS()
class MYPROJECT_06_API UCANS_SkillComboAction : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EComboCondition ComboCondition = EComboCondition::Collsion;

	UPROPERTY(EditAnywhere)
		bool bGoComboInEnd = true;

	UPROPERTY(EditAnywhere)
		int32 Percentage = 1;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

private:
	class AHumanType* Owner;

	FSkillActionData CurSkillAction;

	FVector Location;
	FVector HitLocation;

};

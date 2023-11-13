#pragma once

#include "CoreMinimal.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BowControl.generated.h"

UENUM(BlueprintType)
enum class EBowControl : uint8
{
	Shoot = 0, Draw, Max
};


UCLASS()
class MYPROJECT_06_API UCAnimNotify_BowControl : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EBowControl BowControl = EBowControl::Shoot;

	UPROPERTY(EditAnywhere)
		FHitData SkillHitData;

	UPROPERTY(EditAnywhere)
		bool IsSkillShoot = false;


	UPROPERTY(EditAnywhere)
		bool IsUseTrajectory = false;

private:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
private:
	class AHumanType* Owner;
	UCDoAction_Bow* DoAction_Bow;

};

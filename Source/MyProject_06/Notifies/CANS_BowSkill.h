#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/HumanType.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

#include "Weapons/CWeaponAsset.h"
#include "CANS_BowSkill.generated.h"

UCLASS()
class MYPROJECT_06_API UCANS_BowSkill : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 SkillIndex = 0;

	UPROPERTY(EditAnywhere)
		FHitData SkillHitData;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UCWeaponData* Bow;
	UCDoAction_Bow* Bow_DoAction;
	UCSubAction_Bow* Bow_SubAction;
	ACAttachment_Bow* Bow_Attachment;


	AHumanType* ownerCharacter;

	float Yaw = 0.0f;

};

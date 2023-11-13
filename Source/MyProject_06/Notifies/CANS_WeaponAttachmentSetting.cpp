#include "Notifies/CANS_WeaponAttachmentSetting.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"


FString UCANS_WeaponAttachmentSetting::GetNotifyName_Implementation() const
{
	return "WeaponAttachmentSetting";
}

void UCANS_WeaponAttachmentSetting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<AHumanType>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	CheckTrue(weapon->IsUnarmedMode());

	if(bWeaponUpsideDown)
	{
	

		for (auto Component : MeshComp->GetAttachChildren())
		{

			if (Component->GetName() == "Weapon")
			{
				WeaponMesh = Component;
				WeaponMesh->SetRelativeRotation(FRotator(180, 180, 0));

				
			}
		}

	}



	if(!bWeaponVisible)
	{
		CheckNull(WeaponMesh);
		WeaponMesh->SetVisibility(false);
	}

}

void UCANS_WeaponAttachmentSetting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<AHumanType>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	CheckTrue(weapon->IsUnarmedMode());

	CheckNull(WeaponMesh);

	if (bWeaponUpsideDown)
	{
		WeaponMesh->SetRelativeRotation(FRotator(0, 0, 0));
	}

	if (bWeaponVisible)
	{
		WeaponMesh->SetVisibility(true);
	}

}

void UCANS_WeaponAttachmentSetting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

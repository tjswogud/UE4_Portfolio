#include "Notifies/CANS_Collision.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

FString UCANS_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCANS_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	if(Cast<UCDoAction_Combo>(weapon->GetDoAction())	)
	{
		UCDoAction_Combo* DoAction_Combo = (UCDoAction_Combo*)weapon->GetDoAction();

		DoAction_Combo->GetHitData(HitData);
	}
	
	weapon->GetAttachment()->OnCollisions();
}

void UCANS_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	weapon->GetAttachment()->OffCollisions();
}
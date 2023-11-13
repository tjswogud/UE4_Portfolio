#include "Notifies/CAN_EndAction.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

FString UCAN_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	//CheckNull(weapon);
	//CheckNull(weapon->GetDoAction());

	//if(weapon != nullptr)
	//{
	//	if(weapon->GetDoAction() != nullptr)
	//	{

	//		weapon->GetDoAction()->End_DoAction();
	//	}
	//}

	CheckNull(Cast<AHumanType>(MeshComp->GetOwner()));
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons());
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetDoAction());
	
	Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetDoAction()->End_DoAction();
	
}

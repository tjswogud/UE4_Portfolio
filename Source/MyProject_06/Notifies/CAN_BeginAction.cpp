#include "Notifies/CAN_BeginAction.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Character/HumanType.h"
#include "Weapons/CDoAction.h"

FString UCAN_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";
}

void UCAN_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	CheckNull(Cast<AHumanType>(MeshComp->GetOwner()));
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons());
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetDoAction());

	Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetDoAction()->Begin_DoAction();
}

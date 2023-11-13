#include "Notifies/CANS_Equip.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CEquipment.h"

FString UCANS_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCANS_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetEquipment());

	weapon->GetEquipment()->Begin_Equip();
}

void UCANS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	/*UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetEquipment());

	weapon->GetEquipment()->End_Equip();*/

	CheckFalse(Cast<AHumanType>(MeshComp->GetOwner()));
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons());
	CheckNull(Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetEquipment());

	Cast<AHumanType>(MeshComp->GetOwner())->GetWeapons()->GetEquipment()->End_Equip();
}



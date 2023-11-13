#include "Notifies/CANS_Combo.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

FString UCANS_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCANS_Combo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());


	/*UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);*/

	

	if(Cast<ACEnemy>(MeshComp->GetOwner()))
	{
		if(FMath::RandRange(0, 2) > 0)
		{
			weapon->GetDoAction()->EnableCombo();
		}

	}
	else
	{
		weapon->GetDoAction()->EnableCombo();
	}
}

void UCANS_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());


	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->DisableCombo();
}



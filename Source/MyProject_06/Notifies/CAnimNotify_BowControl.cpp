#include "Notifies/CAnimNotify_BowControl.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CEnemy.h"
#include "Component/CMovementComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/Attachments/CArrow.h"

FString UCAnimNotify_BowControl::GetNotifyName_Implementation() const
{
	return "CAN_BowControl";
}

void UCAnimNotify_BowControl::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(Owner);


	CheckNull(Owner);
	CheckNull(Owner->GetWeapons());
	CheckNull(Owner->GetWeapons()->GetDoAction());

	CheckNull(Cast<UCDoAction_Bow>(Owner->GetWeapons()->GetDoAction()));



	UCWeaponData* Bow;
	if (!Owner->GetWeapons()->IsUnarmedMode())
	{
		Bow = Owner->GetWeapons()->GetCurWeapon();
	}
	else return;

	ACAttachment_Bow* Bow_Attachment = Cast<ACAttachment_Bow>(Bow->GetAttachment());

	DoAction_Bow = Cast<UCDoAction_Bow>(Owner->GetWeapons()->GetDoAction());


	switch (BowControl) {
	case EBowControl::Shoot: 

		if(IsUseTrajectory)
		{
			DoAction_Bow->Shoot(true, &SkillHitData, Bow_Attachment->ArrowVelocity);
		}
		else
		{
			DoAction_Bow->Shoot(IsSkillShoot);
		}
	

		break;
	case EBowControl::Draw: 

		DoAction_Bow->CreateArrow();

		break;
	case EBowControl::Max: 
		
		break;
	default: ;
	}
}

#include "Notifies/CANS_CollisionForExec.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CPlayer.h"
#include "Component/CExecuteComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"


FString UCANS_CollisionForExec::GetNotifyName_Implementation() const
{
	return "CollisionForExec";
}

void UCANS_CollisionForExec::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCExecuteComponent* Execute = CHelpers::GetComponent<UCExecuteComponent>(MeshComp->GetOwner());
	CheckNull(Execute);

	if (!Cast<ACPlayer>(MeshComp->GetOwner())->GetState()->IsExecuteMode()) return;

	if (Execute->GetVictim() == nullptr) return;

	if(Cast<ACEnemy_Boss>(Execute->GetVictim()))
	{
		Execute->GetVictim()->GetStatus()->CurHealth -= (Execute->GetVictim()->GetStatus()->MaxHealth * Percentage) / 2;
	}
	else
	{
		Execute->GetVictim()->GetStatus()->CurHealth -= Execute->GetVictim()->GetStatus()->MaxHealth * Percentage;
	}

	
	if (Execute->GetVictim()->GetStatus()->CurHealth <= 0)
	{
		if(!IsLastAtk)
		{
			Execute->GetVictim()->GetStatus()->CurHealth = 1;
			return;
		}
		else
			Execute->GetVictim()->GetState()->SetDeadMode();
	};

}


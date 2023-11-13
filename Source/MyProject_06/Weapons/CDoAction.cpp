#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "Character/HumanType.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"

UCDoAction::UCDoAction()
{

}



void UCDoAction::BeginPlay(ACAttachment * InAttachment, UCEquipment * InEquipment, ACharacter * InOwner, const TArray<FDoActionData>& InDoActionData, const FDoActionData& InDashActionData, const FDoActionData& InJumpActionData)
{

	OwnerCharacter = Cast<AHumanType>(InOwner);
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);


	DoActionDatas = InDoActionData;
	DashActionData = InDashActionData;
	JumpActionData = InJumpActionData;
}

void UCDoAction::DoAction()
{
	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;
	InUseSkill = false;
	InUseSkillCombo = false;

	if(OwnerCharacter->GetStatus()->GetPlaceType() == EPlaceType::InAir)
	{
		State->SetJumpMode();
	}
	else if(OwnerCharacter->GetStatus()->GetPlaceType() == EPlaceType::OnLand)
	{
		State->SetIdleMode();
	}



	Movement->Move();
	Movement->DisableFixedCamera();
	
}

void UCDoAction::Do_SubAction()
{
	State->SetActionMode();
}

void UCDoAction::Begin_SubAction()
{
	bBeginAction = true;
}

void UCDoAction::End_SubAction()
{
	bBeginAction = false;

	State->SetIdleMode();

	//Movement->Move();
	//Movement->DisableFixedCamera();
}

void UCDoAction::Do_DashAction()
{
	CheckFalse(State->IsIdleMode())

	bBeginAction = true;
	 
	if(!!DashActionData.Montage)
	{
		State->SetActionMode();
		DashActionData.DoAction(OwnerCharacter);
	}

}

void UCDoAction::Do_JumpAction()
{


	bBeginAction = true;


	if (!!JumpActionData.Montage)
	{
	
		State->SetActionMode();
		JumpActionData.DoAction(OwnerCharacter);
	}
}


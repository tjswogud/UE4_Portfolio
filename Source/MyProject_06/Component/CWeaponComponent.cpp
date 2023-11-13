#include "Component/CWeaponComponent.h"

#include <string>

#include "CMovementComponent.h"
#include "CStateComponent.h"
#include "Character/HumanType.h"
#include "Component/CExecuteComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CPlayer.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponData.h"

void UCWeaponComponent::HiddenAttachment(bool IsHidden)
{
	if(IsHidden)
	{
		for (UCWeaponData* data : Datas)
		{
			if(IsValid(data))
				data->GetAttachment()->SetActorHiddenInGame(true);
		}
	}
	else
	{
		for (UCWeaponData* data : Datas)
		{
			if (IsValid(data))
				data->GetAttachment()->SetActorHiddenInGame(false);
		}
	}

}

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AHumanType>(GetOwner());


	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(Owner, &Datas[i]);
	}

}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Cast<ACEnemy_Boss>(Owner)) return;



	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction())
		GetSubAction()->Tick(DeltaTime);

}


bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(Owner)->IsIdleMode();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();
	
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();

}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(IsIdleMode());

	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	if (Cast<ACPlayer>(Owner))
	{
		if (IsValid(Cast<ACPlayer>(Owner)->BowSkillWidget->GetWidget()))
		{
			Cast<ACPlayer>(Owner)->BowSkillWidget->GetWidget()->RemoveFromViewport();
		}


		if (IsValid(Cast<ACPlayer>(Owner)->SwordSkillWidget->GetWidget()))
		{
			Cast<ACPlayer>(Owner)->SwordSkillWidget->GetWidget()->AddToViewport();
		}
	}



	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	if (Cast<ACPlayer>(Owner))
	{


		if (IsValid(Cast<ACPlayer>(Owner)->SwordSkillWidget->GetWidget()))
		{
			Cast<ACPlayer>(Owner)->SwordSkillWidget->GetWidget()->RemoveFromViewport();
		}


		if (IsValid(Cast<ACPlayer>(Owner)->BowSkillWidget->GetWidget()))
		{
			Cast<ACPlayer>(Owner)->BowSkillWidget->GetWidget()->AddToViewport();
		}

	}



	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::DoAction()
{
	

	if (!IsValid(GetDoAction())) return;
	else
	{
		GetDoAction()->DoAction();
	}


		
}

void UCWeaponComponent::DashAction()
{
	if (!IsValid(GetDoAction())) return;
	

	if (!!GetDoAction())
		GetDoAction()->Do_DashAction();
}

void UCWeaponComponent::JumpAction()
{
	if (!IsValid(GetDoAction())) return;
		

	if (GetDoAction() != nullptr)
		GetDoAction()->Do_JumpAction();
}

void UCWeaponComponent::SubAction_Pressed()
{

	if (!IsValid(GetSubAction())) return;
	else
	{
		GetSubAction()->Pressed();
	}

	//if (!!GetSubAction())
		
}

void UCWeaponComponent::SubAction_Released()
{
	if (!IsValid(GetSubAction())) return;
	else
	{
		GetSubAction()->Released();
	}
		
}

void UCWeaponComponent::SkillAction(int Inindex)
{
	

	if (!IsUnarmedMode())
	{
		if(Datas[(int32)Type]->GetSkillActionDatas().Num() - 1 >= Inindex)
		{
			
			Datas[(int32)Type]->GetSkillActionDatas()[Inindex].DoSkillAction(Owner);
			Datas[(int32)Type]->GetDoAction()->SkillIndex = Inindex;
		}
	}
}




void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}

	//if (!!DataAssets[(int32)InType])
	//{
	//	DataAssets[(int32)InType]->GetEquipment()->Equip();

	//	ChangeType(InType);
	//}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);
}


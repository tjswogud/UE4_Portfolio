#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "Weapons/CWeaponData.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	DoActionClass = UCDoAction::StaticClass();
	SubActionClass = UCSubAction::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter * InOwner, UCWeaponData** OutWeaponData)
{


	FActorSpawnParameters params;

	ACAttachment* Attachment = nullptr;
	if (!!AttachmentClass)
	{
	
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* Equipment = nullptr;
	if (!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

		if (!!Attachment)
		{ 
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}


	UCDoAction* DoAction = nullptr;
	if (!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas, DashActionData, JumpActionData);

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if (!!Equipment)
		{
			Equipment->OnEquipmentEquip.AddDynamic(DoAction, &UCDoAction::OnEquip);
			Equipment->OnEquipmentBeginEquip.AddDynamic(DoAction, &UCDoAction::OnBeginEquip);
			Equipment->OnEquipmentEndEquip.AddDynamic(DoAction, &UCDoAction::OnEndEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(DoAction, &UCDoAction::OnUnequip);
		}
	}


	UCSubAction* SubAction = nullptr;
	if(!!SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this, SubActionClass);
		SubAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = Attachment;
	(*OutWeaponData)->Equipment = Equipment;
	(*OutWeaponData)->DoAction = DoAction;
	(*OutWeaponData)->SubAction = SubAction;
	(*OutWeaponData)->SkillActionDatas = SkillActionDatas;
	(*OutWeaponData)->DeflectActionDatas= DeflectActionDatas;
	(*OutWeaponData)->ExecuteDataTable = ExecuteDataTable;
}

void UCWeaponAsset::CopyData(const UCWeaponAsset& Other, class ACharacter* Owner)
{
	//AttachmentClass = Other.AttachmentClass;
	//EquipmentClass = Other.EquipmentClass;
	//DoActionClass = Other.DoActionClass;
	//SubActionClass = Other.SubActionClass;

	//SubAction = Other.SubAction;
	//DoAction = Other.DoAction;
	//EquipmentData = Other.EquipmentData;
	//DoActionDatas = Other.DoActionDatas;
	//DashActionData = Other.DashActionData;
	//JumpActionData = Other.JumpActionData;
	//SkillActionDatas = Other.SkillActionDatas;
	//DeflectActionDatas = Other.DeflectActionDatas;

	//ExecuteDataTable = Other.ExecuteDataTable;

	//BeginPlay(Owner);

}

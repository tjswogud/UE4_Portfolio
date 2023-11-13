#include "Notifies/CANS_SkillComboAction.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CPlayer.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/CWeaponData.h"

FString UCANS_SkillComboAction::GetNotifyName_Implementation() const
{
	return "SkillComboAction";
}

void UCANS_SkillComboAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = (Cast<AHumanType>(MeshComp->GetOwner()));

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());


	Owner->GetWeapons()->GetCurWeapon()->GetDoAction()->InUseSkill = true;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentMontage)
		{
			for (FSkillActionData Data : Owner->GetWeapons()->GetCurWeapon()->GetSkillActionDatas())
			{
				if(Data.Montage == CurrentMontage)
				{
					CurSkillAction = Data;
					break;
				}
			}
		}
	}
}

void UCANS_SkillComboAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	CheckTrue(Owner->GetWeapons()->IsUnarmedMode());

	Owner->GetWeapons()->GetCurWeapon()->GetDoAction()->InUseSkill = false;
	Owner->GetWeapons()->GetCurWeapon()->GetDoAction()->InUseSkillCombo = true;



	if(CurSkillAction.bCanCombo)
	{
		FRotator rotator = FRotator::ZeroRotator;

		switch (ComboCondition)
		{
		case EComboCondition::Collsion: 
			rotator	= UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), HitLocation);
			Owner->SetActorRotation(rotator);
			Owner->SetActorLocation(Location);
			break;
		case EComboCondition::Command: 
			
			break;
		case EComboCondition::Random: 

			break;
		default: break;
		}




		CurSkillAction.DoComboAction(Owner);
	}

}

void UCANS_SkillComboAction::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	CheckTrue(Owner->GetWeapons()->IsUnarmedMode());

	int32 idx = 0;
	switch (ComboCondition)
	{
		case EComboCondition::Collsion: 
			if (Cast<UCDoAction_Combo>(Owner->GetWeapons()->GetCurWeapon()->GetDoAction()))
			{
				if (Cast<UCDoAction_Combo>(Owner->GetWeapons()->GetCurWeapon()->GetDoAction())->GetHittedActors().Num() != 0)
				{
					Location = Cast<UCDoAction_Combo>(Owner->GetWeapons()->GetCurWeapon()->GetDoAction())->GetHittedActors()[0]->GetActorLocation() + Cast<UCDoAction_Combo>(Owner->GetWeapons()->GetCurWeapon()->GetDoAction())->GetHittedActors()[0]->GetActorForwardVector() * 50;
					HitLocation = Cast<UCDoAction_Combo>(Owner->GetWeapons()->GetCurWeapon()->GetDoAction())->GetHittedActors()[0]->GetActorLocation();

					CurSkillAction.bCanCombo = true;
				}
			}


			break;
		case EComboCondition::Command:
			if(Cast<ACPlayer>(Owner))
			{
				if(Cast<APlayerController>(Owner->GetController())->IsInputKeyDown(EKeys::LeftMouseButton))
				{
					CurSkillAction.bCanCombo = true;
					if(!bGoComboInEnd)
					{
						Owner->GetWeapons()->GetCurWeapon()->GetDoAction()->InUseSkill = false;
						Owner->GetWeapons()->GetCurWeapon()->GetDoAction()->InUseSkillCombo = true;
						CurSkillAction.DoComboAction(Owner);
					}
				}

			}



			
			break;
		case EComboCondition::Random:



			idx = UKismetMathLibrary::RandomInteger(Percentage);

			if (idx == 0)
			{
				CurSkillAction.bCanCombo = true;
			}


			
			break;
		default: ;
	}

	

}

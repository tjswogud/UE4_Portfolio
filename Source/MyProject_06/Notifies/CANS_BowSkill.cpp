#include "Notifies/CANS_BowSkill.h"
#include "Global.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CWeaponData.h"

FString UCANS_BowSkill::GetNotifyName_Implementation() const
{
	return "CANS_BowSkill";
}

void UCANS_BowSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);


	ownerCharacter = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);

	if (!ownerCharacter->GetWeapons()->IsUnarmedMode())
	{
		Bow = ownerCharacter->GetWeapons()->GetCurWeapon();
	}
	else return;
	
	Bow_Attachment = Cast<ACAttachment_Bow>(Bow->GetAttachment());
	Bow_DoAction = Cast<UCDoAction_Bow>(Bow->GetDoAction());
	Bow_SubAction = Cast<UCSubAction_Bow>(Bow->GetSubAction());




	CheckNull(Bow_Attachment);
	CheckNull(Bow_DoAction);
	CheckNull(Bow_SubAction);
	

	switch (SkillIndex)
	{
	case 0: 


		break;
	case 1:

		ownerCharacter->GetMovement()->DisableControlRotation();

		break;
	case 2: 

		Bow_DoAction->Shoot(true, &SkillHitData, Bow_Attachment->ArrowVelocity);

		break;
	case 3: 
		
		break;
	}
}

void UCANS_BowSkill::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);


	ownerCharacter = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);
	if (!ownerCharacter->GetWeapons()->IsUnarmedMode())
	{
		Bow = ownerCharacter->GetWeapons()->GetCurWeapon();
	}
	else return;

	switch (SkillIndex)
	{
	case 0:


		break;
	case 1:



		Yaw = UKismetMathLibrary::Lerp(Yaw, 360.0f, 50.0f);

		ownerCharacter->SetActorRotation(UKismetMathLibrary::MakeRotator(ownerCharacter->GetActorRotation().Roll, ownerCharacter->GetActorRotation().Pitch, ownerCharacter->GetActorRotation().Yaw + Yaw));

		if(Bow_DoAction->GetAttachedArrow() == nullptr)
		{
			Bow_DoAction->CreateArrow();
		}

		Bow_DoAction->Shoot(false);

		Bow_DoAction->OnArrowEndPlay(Bow_DoAction->Arrows.Last());


		break;
	case 2:

		break;
	case 3:

		break;
	}
}

void UCANS_BowSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{


	ownerCharacter = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);
	if (!ownerCharacter->GetWeapons()->IsUnarmedMode())
	{
		Bow = ownerCharacter->GetWeapons()->GetCurWeapon();
	}
	else return;

	switch (SkillIndex)
	{
	case 0:
		
		Bow_DoAction->Shoot(true, &SkillHitData);

		break;
	case 1:

		ownerCharacter->GetMovement()->EnableControlRotation();

		break;
	case 2:

		break;
	case 3:

		break;
	}

	Super::NotifyEnd(MeshComp, Animation);
}

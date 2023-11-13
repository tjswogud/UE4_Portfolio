#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"



void UCDoAction_Combo::GetHitData(FHitData InData)
{

	LocalHitData = InData;

	//LocalHitData->HittedType = InData.HittedType;
	//LocalHitData->HitIndex = InData.HitIndex;
	//LocalHitData->PlayRate = InData.PlayRate;
	//LocalHitData->Power = InData.Power;
	//LocalHitData->Launch = InData.Launch;
	//LocalHitData->StopTime= InData.StopTime;
	//LocalHitData->Sound= InData.Sound;
	//LocalHitData->Effect = InData.Effect;
	//LocalHitData->EffectLocation = InData.EffectLocation;
	//LocalHitData->EffectScale = InData.EffectScale;
}

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	if(Cast<ACEnemy_Boss>(OwnerCharacter))
	{
		if(OwnerCharacter->GetState()->IsRopeActionMode())
		{
			Super::DoAction();
			DoActionDatas[Index].DoAction(OwnerCharacter);


			Cast<ACEnemy_AIController_Boss>(OwnerCharacter->GetController())->GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Alert));
			return;
		}

	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::Do_DashAction()
{
	Super::Do_DashAction();
}

void UCDoAction_Combo::Do_JumpAction()
{
	Super::Do_JumpAction();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	if(Cast<ACPlayer>(OwnerCharacter))
	{
		if(!InUseSkill)
		{
			if (LocalHitData.HittedType == EHittedType::StrongHit)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes += 20;
			}
			else if (LocalHitData.HittedType == EHittedType::NormalHit)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes += 10;
			}

			if (Cast<ACPlayer>(OwnerCharacter)->CurRes > Cast<ACPlayer>(OwnerCharacter)->MaxRes)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes = Cast<ACPlayer>(OwnerCharacter)->MaxRes;
			}
		}
	}

	
	LocalHitData.SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();


	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();


		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	//if (!!candidate)
	//{
	//	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
	//	FRotator target = FRotator(0, rotator.Yaw, 0);

	//	AController* controller = OwnerCharacter->GetController<AController>();
	//	CheckNull(controller);

	//	controller->SetControlRotation(target);
	//}

	LocalHitData = FHitData();

	Hitted.Empty();
}

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

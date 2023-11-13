#include "AI/Task/CBT_PatternandAction.h"
#include "AIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "AI/CEnemy_AIController_Range.h"
#include "Character/CEnemy_Melee.h"
#include "Character/CEnemy_Range.h"
#include "Character/HumanType.h"
#include "Weapons/CWeaponAsset.h"


UCBT_PatternandAction::UCBT_PatternandAction()
{
	bNotifyTick = true;
	NodeName = TEXT("PatternAndAction");
}

EBTNodeResult::Type UCBT_PatternandAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllingPawn = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	if(Cast<ACEnemy_Melee>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController::Behavior;
		TargetName = ACEnemy_AIController::Target;
	}
	else if(Cast<ACEnemy_Melee>(ControllingPawn))
	{

		BehaviorName = ACEnemy_AIController_Range::Behavior;
		TargetName = ACEnemy_AIController_Range::Target;
	}
	else
	{
		BehaviorName = ACEnemy_AIController_Boss::Behavior;
		TargetName = ACEnemy_AIController_Boss::Target;
	}


	AHumanType* Target = Cast<AHumanType>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetName));
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(ControllingPawn->GetActorLocation()), CHelpers::GetVector2D(Target->GetActorLocation()));

	ControllingPawn->SetActorRotation(Rotator);





	if(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) == (uint8)EAI_StateType::Action)
	{
		switch (ControllingPawn->ActionType) {
		case EAI_EnemyActionType::Attack:
			if (ControllingPawn->GetWeapons()->IsUnarmedMode())
			{
				switch (Weapon) {
				case EWeaponType::Sword: 
					ControllingPawn->GetWeapons()->SetSwordMode();
					break;
				case EWeaponType::Bow: 
					ControllingPawn->GetWeapons()->SetBowMode();
					break;
				case EWeaponType::Max: break;
				default: ;
				}
				
			}


			ControllingPawn->AI_Attack();

			return EBTNodeResult::Succeeded;
		case EAI_EnemyActionType::Block: break;
		case EAI_EnemyActionType::Skill: 

			if (ControllingPawn->GetWeapons()->IsUnarmedMode())
			{
				switch (Weapon) {
				case EWeaponType::Sword:
					ControllingPawn->GetWeapons()->SetSwordMode();
					break;
				case EWeaponType::Bow:
					ControllingPawn->GetWeapons()->SetBowMode();
					break;
				case EWeaponType::Max: break;
				default:;
				}
			}

			CheckFalseResult(ControllingPawn->GetState()->IsIdleMode(), EBTNodeResult::Failed)

			ControllingPawn->LookAtTarget();

			switch (ControllingPawn->DistancefromTarget)
			{
				case EDistancefromTarget::Near:	
					ControllingPawn->UseSkill(ControllingPawn->GetRandomIndexData(ControllingPawn->NearSkills));

					break;
				case EDistancefromTarget::Middle: 
					ControllingPawn->UseSkill(ControllingPawn->GetRandomIndexData(ControllingPawn->MiddleSkills));

					break;
				case EDistancefromTarget::Far: 
					ControllingPawn->UseSkill(ControllingPawn->GetRandomIndexData(ControllingPawn->FarSkills));
					break;
				default: break;
			}

			return EBTNodeResult::Succeeded;
		case EAI_EnemyActionType::Avoid: break;
		case EAI_EnemyActionType::Max: break;
		default:;
		}

	}
	else
	{
		return  EBTNodeResult::Failed;
	}



	return  EBTNodeResult::Failed;
}

EBTNodeResult::Type UCBT_PatternandAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBT_PatternandAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

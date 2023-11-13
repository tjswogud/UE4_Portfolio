#include "AI/Task/CBT_StateTask.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "AI/CEnemy_AIController_Range.h"
#include "Character/CEnemy.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CEnemy_Melee.h"
#include "Character/CEnemy_Range.h"
#include "Component/CMontagesComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"


UCBT_StateTask::UCBT_StateTask()
{
	bNotifyTick = true;
	NodeName = TEXT("StateTask");


}

EBTNodeResult::Type UCBT_StateTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{



	ControllingPawn = Cast<AHumanType>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	CLog::Print("ExecuteTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
	CLog::Log("ExecuteTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();



	if (Cast<ACEnemy_Range>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController_Range::Behavior;
	}
	else if (Cast<ACEnemy_Melee>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController::Behavior;
	}
	else if (Cast<ACEnemy_Boss>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController_Boss::Behavior;
	}

	switch (State) {
	case EStateType::Idle:

		break;
	case EStateType::Dodge:		

		if (ControllingPawn->GetWeapons()->IsUnarmedMode())
		{
			ControllingPawn->GetWeapons()->SetSwordMode();
			return EBTNodeResult::InProgress;
		}

		ControllingPawn->GetMontages()->PlayAnimMontage(EStateType::Dodge, UKismetMathLibrary::RandomInteger(3));

		return EBTNodeResult::Succeeded;
		
		break;
	case EStateType::Equip:



		if (!ControllingPawn->GetWeapons()->IsUnarmedMode())
		{
			return EBTNodeResult::Succeeded;
		}
		else
		{
			if(Cast<ACEnemy_Range>(ControllingPawn))
			{
				ControllingPawn->GetWeapons()->SetBowMode();

			}
			else if(Cast<ACEnemy_Melee>(ControllingPawn))
			{
				ControllingPawn->GetWeapons()->SetSwordMode();
			}
			else if(Cast<ACEnemy_Boss>(ControllingPawn))
			{
				ControllingPawn->GetWeapons()->SetSwordMode();
			}

			return EBTNodeResult::InProgress;
		}



		

		break;
	case EStateType::Hitted:
		if (Cast<ACEnemy_Range>(ControllingPawn))
		{
			if (!ControllingPawn->GetState()->IsHittedMode())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}
		else if (Cast<ACEnemy_Boss>(ControllingPawn))
		{
			if (!ControllingPawn->GetState()->IsHittedMode())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}


		break;
	case EStateType::Dead:		

		if (Cast<ACEnemy_Melee>(ControllingPawn))
		{
			Cast<ACEnemy_AIController>(ControllingPawn->GetController())->StopAI();
		}
		else if (Cast<ACEnemy_Range>(ControllingPawn))
		{
			Cast<ACEnemy_AIController_Range>(ControllingPawn->GetController())->StopAI();
		}
		else if (Cast<ACEnemy_Boss>(ControllingPawn))
		{
			Cast<ACEnemy_AIController_Boss>(ControllingPawn->GetController())->StopAI();
		}


		ControllingPawn->GetMesh()->SetSimulatePhysics(true);


		return EBTNodeResult::Succeeded;

		break;
	case EStateType::Action:	break;
	case EStateType::Ground:	
		
		if (Cast<ACEnemy_Range>(ControllingPawn))
		{
			if (!ControllingPawn->GetState()->IsGroundMode())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}
		else if (Cast<ACEnemy_Boss>(ControllingPawn))
		{
			if (!ControllingPawn->GetState()->IsGroundMode())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}
		break;
	case EStateType::Rise:		break;
	case EStateType::Sneak:		break;
	case EStateType::Hide:		break;
	case EStateType::Groggy:	

		if (Cast<ACEnemy_Range>(ControllingPawn))
		{
			if (!ControllingPawn->GetStatus()->CheckGroggy())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}

		else if (Cast<ACEnemy_Boss>(ControllingPawn))
		{
			if (!ControllingPawn->GetStatus()->CheckGroggy())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
			}

		}
		break;
	case EStateType::Execute:	break;
	case EStateType::Parried:	break;
	case EStateType::Max:		break;
	case EStateType::Jump: break;
	case EStateType::RopeAction:



		if (Cast<ACEnemy_Boss>(ControllingPawn))
		{
			Cast<ACEnemy_Boss>(ControllingPawn)->AI_RopeAction();
			return EBTNodeResult::Succeeded;
		}

		break;
	case EStateType::Climb: break;
	default:;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UCBT_StateTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (Cast<ACEnemy_Range>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController_Range::Behavior;
	}
	else
	{
		BehaviorName = ACEnemy_AIController::Behavior;
	}

	CLog::Print("AbortTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
	CLog::Log("AbortTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));

	switch (State) {
	case EStateType::Idle:
		return EBTNodeResult::Succeeded;
		break;
	case EStateType::Dodge:		
		
		
		break;
	case EStateType::Equip:

		ControllingPawn->GetWeapons()->GetCurWeapon()->GetEquipment()->End_Equip();
		return EBTNodeResult::Succeeded;
		break;
	case EStateType::Hitted:	


		
		break;
	case EStateType::Dead:		break;
	case EStateType::Action:	break;
	case EStateType::Ground:	break;
	case EStateType::Rise:		break;
	case EStateType::Sneak:		break;
	case EStateType::Hide:		break;
	case EStateType::Groggy:	break;
	case EStateType::Execute:	break;
	case EStateType::Parried:	break;
	case EStateType::Max:		break;
	default:;
	}
	//	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBT_StateTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Cast<ACEnemy_Range>(ControllingPawn))
	{
		BehaviorName = ACEnemy_AIController_Range::Behavior;
	}
	else
	{
		BehaviorName = ACEnemy_AIController::Behavior;
	}


	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	CLog::Print("TickTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
	CLog::Log("TickTask : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));




	switch (State)
	{
	case EStateType::Idle: break;
	case EStateType::Dodge: break;
	case EStateType::Equip: break;
	case EStateType::Hitted:




		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Hitted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		}

		break;
	case EStateType::Dead: break;
	case EStateType::Action: break;
	case EStateType::Ground:
		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Ground)
		{

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		}
		else
		{

			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

		}

		break;

	case EStateType::Rise: break;
	case EStateType::Sneak: break;
	case EStateType::Hide: break;
	case EStateType::Groggy:
		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Groggy)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		}
		break;
	case EStateType::Execute:
		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Groggy)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		}

		break;
	case EStateType::Parried: break;
	case EStateType::Max: break;
	default:;
	}
}


















//
//UCBT_StateTask::UCBT_StateTask()
//{
//	bNotifyTick = true;
//	NodeName = TEXT("StateTask");
//
//
//}
//
//EBTNodeResult::Type UCBT_StateTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	
//	ControllingPawn = Cast<AHumanType>(OwnerComp.GetAIOwner()->GetPawn());
//	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
//
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
//	if (NavSystem == nullptr) return EBTNodeResult::Failed;
//
//	if (Cast<ACEnemy_Range>(ControllingPawn))
//	{
//		BehaviorName = ACEnemy_AIController_Range::Behavior;
//	}
//	else
//	{
//		BehaviorName = ACEnemy_AIController::Behavior;
//	}
//
//
//	switch (State) {
//	case EStateType::Idle:	
//		
//		break;
//	case EStateType::Dodge:		break;
//	case EStateType::Equip:
//
//		
//
//		if(!ControllingPawn->GetWeapons()->IsUnarmedMode())
//		{
//				
//		}
//		else
//		{
//			if(Cast<ACEnemy_Range>(ControllingPawn))
//			{
//				ControllingPawn->GetWeapons()->SetBowMode();
//
//
//			}
//			else if(Cast<ACEnemy_Melee>(ControllingPawn))
//			{
//				ControllingPawn->GetWeapons()->SetSwordMode();
//			}
//			
//		}
//
//
//
//		return EBTNodeResult::Succeeded;
//		
//		break;
//	case EStateType::Hitted:	
//
//
//		
//
//		CLog::Print("Execute Task : " +	UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//		CLog::Log("Execute Task : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//	
//
//		break;
//	case EStateType::Dead:		break;
//	case EStateType::Action:	break;
//	case EStateType::Ground:	break;
//	case EStateType::Rise:		break;
//	case EStateType::Sneak:		break;
//	case EStateType::Hide:		break;
//	case EStateType::Groggy:	break;
//	case EStateType::Execute:	break;
//	case EStateType::Parried:	break;
//	case EStateType::Max:		break;
//	default: ;
//	}
//
//	return EBTNodeResult::InProgress;
//}
//
//EBTNodeResult::Type UCBT_StateTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//
//	switch (State) {
//	case EStateType::Idle:
//		return EBTNodeResult::Succeeded;
//		break;
//	case EStateType::Dodge:		break;
//	case EStateType::Equip:
//
//		ControllingPawn->GetWeapons()->GetCurWeapon()->GetEquipment()->End_Equip();	
//		return EBTNodeResult::Succeeded;
//		break;
//	case EStateType::Hitted:
//
//
//		CLog::Print("Abort Task : " +	UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//		CLog::Log("Abort Task : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//
//		break;
//	case EStateType::Dead:		break;
//	case EStateType::Action:	break;
//	case EStateType::Ground:	break;
//	case EStateType::Rise:		break;
//	case EStateType::Sneak:		break;
//	case EStateType::Hide:		break;
//	case EStateType::Groggy:	break;
//	case EStateType::Execute:	break;
//	case EStateType::Parried:	break;
//	case EStateType::Max:		break;
//	case EStateType::Jump: break;
//	case EStateType::RopeAction: break;
//	case EStateType::Climb: break;
//	default:;
//	}
//	//	
//	return Super::AbortTask(OwnerComp, NodeMemory);
//}
//
//void UCBT_StateTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//
//
//	
//
//
//	switch (State)
//	{
//	case EStateType::Idle: break;
//	case EStateType::Dodge: break;
//	case EStateType::Equip: break;
//	case EStateType::Hitted:
//
//
//
//		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Hitted)
//		{
//
//
//			CLog::Print("Tick Task : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//			CLog::Log("Tick Task : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName)));
//
//
//			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//		}
//		else
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
//		}
//
//		
//		break;
//	case EStateType::Dead: break;
//	case EStateType::Action: break;
//	case EStateType::Ground:
//
//		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Ground)
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//		}
//		else
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
//		}
//
//
//
//		
//		break;
//
//	case EStateType::Rise: break;
//	case EStateType::Sneak: break;
//	case EStateType::Hide: break;
//	case EStateType::Groggy:
//
//		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Groggy)
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//		}
//		else
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
//		}
//
//		break;
//	case EStateType::Execute:
//
//		if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) != (uint8)EAI_StateType::Execute)
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//		}
//		else
//		{
//			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
//		}
//
//
//		break;
//	case EStateType::Parried: break;
//	case EStateType::Max: break;
//	default: ;
//	}
//}

#include "AI/CBTService_Enemy.h"
#include "AIController.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CEnemy.h"

UCBTService_Enemy::UCBTService_Enemy()
{
	NodeName = TEXT("Enemy_Service");
	Interval = 1.0f;

	
}

void UCBTService_Enemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;

	CheckNull(OwnerComp.GetBlackboardComponent());

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	//CLog::Log(UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)BB->GetValueAsEnum(ACEnemy_AIController::Behavior)));

	CLog::Print("Service : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(ACEnemy_AIController::Behavior)));
	CLog::Log("Service : " + UEnum::GetValueAsString<EAI_StateType>((EAI_StateType)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(ACEnemy_AIController::Behavior)));

	ACEnemy* Owner = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsGroggyMode() ||
		Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsGroundMode() ||
		Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsHittedMode() ||
		Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsExecuteMode() ||
		Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsDeadMode())
	{
		if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsGroggyMode())
		{
			BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Groggy);
		}
		else if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsGroundMode())
		{
			BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Ground);
		}
		else if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsHittedMode())
		{
			BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Hitted);
		}
		else if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsExecuteMode())
		{
			BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Execute);
		}
		else if (Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsDeadMode())
		{
			BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Dead);
		}
	}
	else if(IsValid(BB->GetValueAsObject(ACEnemy_AIController::Target)) && BB->GetValueAsEnum(ACEnemy_AIController::Behavior) != (uint8)EAI_StateType::Groggy && BB->GetValueAsEnum(ACEnemy_AIController::Behavior) != (uint8)EAI_StateType::Ground && BB->GetValueAsEnum(ACEnemy_AIController::Behavior) != (uint8)EAI_StateType::Hitted && BB->GetValueAsEnum(ACEnemy_AIController::Behavior) != (uint8)EAI_StateType::Execute)
	{
		

		if ((Cast<AHumanType>(BB->GetValueAsObject("Target"))->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()).Size() < ActionRange)
		{
			//타겟과의 거리가 가까울 때
			
			Owner->DistancefromTarget = EDistancefromTarget::Near;

			if(Owner->NearPatterns.Num() == 0)
			{

				Owner->ActionType = static_cast<EAI_EnemyActionType>(FMath::RandRange(0, Owner->NearActions.Num()));
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior, static_cast<uint8>(EAI_StateType::Approach));

			}
			else
			{
				Owner->PatternType = Owner->NearPatterns.GetData()[FMath::RandRange(0, Owner->NearPatterns.Num()-1)];
				Owner->ActionType = Owner->NearActions.GetData()[FMath::RandRange(0, Owner->NearActions.Num() - 1)];
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)Owner->PatternType);

			}
		}
		else if((Cast<AHumanType>(BB->GetValueAsObject("Target"))->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()).Size() < AlertRange && (Cast<AHumanType>(BB->GetValueAsObject("Target"))->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()).Size() > ActionRange)
		{
			//타겟과의 거리가 중간일 때

			Owner->DistancefromTarget = EDistancefromTarget::Middle;

			if (Owner->MiddlePatterns.Num() == 0)
			{
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior, static_cast<uint8>(EAI_StateType::Approach));
				Owner->ActionType = static_cast<EAI_EnemyActionType>(FMath::RandRange(0, Owner->MiddleActions.Num()));
			}
			else
			{
				Owner->PatternType = Owner->MiddlePatterns.GetData()[FMath::RandRange(0, Owner->MiddlePatterns.Num()-1)];
				Owner->ActionType = Owner->MiddleActions.GetData()[FMath::RandRange(0, Owner->MiddleActions.Num() - 1)];
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)Owner->PatternType);
			}
		}
		else if((Cast<AHumanType>(BB->GetValueAsObject("Target"))->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()).Size() > AlertRange)
		{
			//타겟과의 거리가 멀 때

		
			Owner->DistancefromTarget = EDistancefromTarget::Far;

			if (Owner->FarPatterns.Num() == 0)
			{
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior, static_cast<uint8>(EAI_StateType::Approach));
				Owner->ActionType = static_cast<EAI_EnemyActionType>(FMath::RandRange(0, Owner->FarActions.Num()));
			}
			else
			{
				Owner->PatternType = Owner->FarPatterns.GetData()[FMath::RandRange(0, Owner->FarPatterns.Num()-1)];
				Owner->ActionType = Owner->FarActions.GetData()[FMath::RandRange(0, Owner->FarActions.Num() - 1)];
				Owner->InPrevState = static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior));
				BB->SetValueAsEnum(ACEnemy_AIController::Behavior,(uint8)Owner->PatternType);
			}
		}

	}
	else if(Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn())->GetState()->IsIdleMode())
	{
		Owner->SetPrevState(static_cast<EAI_StateType>(BB->GetValueAsEnum(ACEnemy_AIController::Behavior)));
		BB->SetValueAsEnum("Behavior", static_cast<uint8>(EAI_StateType::Patrol));
	}
	
}



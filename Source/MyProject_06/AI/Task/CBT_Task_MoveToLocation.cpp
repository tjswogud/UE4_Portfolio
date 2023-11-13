#include "AI/Task/CBT_Task_MoveToLocation.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "AI/CEnemy_AIController_Range.h"
#include "AI/CPatrolPath.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CEnemy_Melee.h"
#include "Character/CEnemy_Range.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UCBT_Task_MoveToLocation::UCBT_Task_MoveToLocation()
{
	bNotifyTick = true;
	NodeName = TEXT("MoveToLocation");
}

EBTNodeResult::Type UCBT_Task_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	if(Cast<ACEnemy_Range>(ControllingPawn))
	{
		TargetLocationName = ACEnemy_AIController_Range::TargetLocation;
		BehaviorName = ACEnemy_AIController_Range::Behavior;
		TargetName = ACEnemy_AIController_Range::Target;
	}
	else if(Cast<ACEnemy_Melee>(ControllingPawn))
	{
		TargetLocationName = ACEnemy_AIController::TargetLocation;
		BehaviorName = ACEnemy_AIController::Behavior;
		TargetName = ACEnemy_AIController::Target;
	}
	else if (Cast<ACEnemy_Boss>(ControllingPawn))
	{
		TargetLocationName = ACEnemy_AIController_Boss::TargetLocation;
		BehaviorName = ACEnemy_AIController_Boss::Behavior;
		TargetName = ACEnemy_AIController_Boss::Target;
	}

	if ((OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationName) - ControllingPawn->GetActorLocation()).IsNearlyZero())
	{

		return EBTNodeResult::Succeeded;
	}
	else
	{

		return EBTNodeResult::InProgress;
	}

}

EBTNodeResult::Type UCBT_Task_MoveToLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	return Super::AbortTask(OwnerComp, NodeMemory);

}

void UCBT_Task_MoveToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(Cast<AHumanType>(OwnerComp.GetAIOwner()->GetPawn())) //owner가 있는 지 검사
	{
		ACEnemy* Owner = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());

		if(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BehaviorName) == (uint8)EAI_StateType::Patrol) 
		{
			Cast<AHumanType>(Owner)->GetMovement()->SetSpeed(ESpeedType::Walk);
			if (Owner->GetMovement()->CanMove())//owner가 움직일 수 있는 지 검사
			{
				FVector Velocity = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationName) - Owner->GetActorLocation();
				FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(Owner->GetActorLocation()), CHelpers::GetVector2D(OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationName)));
				Owner->SetActorRotation(Rotator);
				Owner->AddMovementInput(Velocity, 1.0f);
			}

			if (CHelpers::GetVector2D(OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationName) - CHelpers::GetVector2D(Owner->GetActorLocation())).Size() < 100)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			}
		}
		else
		{
			Cast<AHumanType>(Owner)->GetMovement()->SetSpeed(ESpeedType::Run);
			if (Owner->GetMovement()->CanMove())//owner가 움직일 수 있는 지 검사
			{
				AHumanType* Target = Cast<AHumanType>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetName));
				FVector Velocity = Target->GetActorLocation() - Owner->GetActorLocation();
				FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(Owner->GetActorLocation()), CHelpers::GetVector2D(Target->GetActorLocation()));

				Owner->SetActorRotation(Rotator);
				Owner->AddMovementInput(Velocity, 1.0f);
			}

			if (CHelpers::GetVector2D(OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationName) - CHelpers::GetVector2D(Owner->GetActorLocation())).Size() < 100)
			{

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			}
		}


	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}

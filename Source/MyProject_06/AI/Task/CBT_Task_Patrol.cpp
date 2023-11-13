#include "AI/Task/CBT_Task_Patrol.h"
#include "Global.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "AI/CEnemy_AIController_Range.h"
#include "AI/CPatrolPath.h"
#include "Character/CEnemy.h"
#include "Component/CMovementComponent.h"
#include "Components/SplineComponent.h"

UCBT_Task_Patrol::UCBT_Task_Patrol()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UCBT_Task_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FName TargetLocation;
	if(Cast<ACEnemy_AIController>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController::TargetLocation;
	}
	else if(Cast<ACEnemy_AIController_Range>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController_Range::TargetLocation;
	}
	else if(Cast<ACEnemy_AIController_Boss>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController_Boss::TargetLocation;
	}


	ACEnemy* ControllingPawn = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FVector Origin = ControllingPawn->GetActorLocation();
	FNavLocation NextPatrol(Origin);

	ControllingPawn->GetMovement()->SetSpeed(ESpeedType::Walk);

	if(!!ControllingPawn->GetPatrolPath())
	{
		FVector moveToPoint;

		if(ControllingPawn->InPrevState != EAI_StateType::Patrol)
		{
			int32 nearPoint = 10000;
			for (int32 i = 0; i < ControllingPawn->GetPatrolPath()->GetSpline()->GetNumberOfSplinePoints(); i++)
			{
				if ((ControllingPawn->GetPatrolPath()->GetSpline()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - ControllingPawn->GetActorLocation()).Size() < nearPoint)
				{
					nearPoint = (ControllingPawn->GetPatrolPath()->GetSpline()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - ControllingPawn->GetActorLocation()).Size();
					ControllingPawn->GetPatrolPath()->Index = i;
				}

			}
		}
		else
		{
			ControllingPawn->GetPatrolPath()->UpdateIndex();
		}

		moveToPoint = ControllingPawn->GetPatrolPath()->GetMoveTo();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation, moveToPoint);
		
		ControllingPawn->InPrevState = EAI_StateType::Patrol;


		if (bDebugMode)
			DrawDebugSphere(ControllingPawn->GetWorld(), moveToPoint, 25, 25, FColor::Green, true, 5);

		return EBTNodeResult::Succeeded;
	}

	while (true)
	{
		if (NavSystem->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), RandomRadius, NextPatrol))
			break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation, CHelpers::GetVector2D(NextPatrol.Location));



	if (bDebugMode)
		DrawDebugSphere(ControllingPawn->GetWorld(), NextPatrol.Location, 25, 25, FColor::Green, true, 5);


	return EBTNodeResult::Succeeded;
}

void UCBT_Task_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FName TargetLocation;
	if (Cast<ACEnemy_AIController>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController::TargetLocation;
	}
	else if (Cast<ACEnemy_AIController_Range>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController_Range::TargetLocation;
	}
	else if (Cast<ACEnemy_AIController_Boss>(OwnerComp.GetOwner()->GetInstigatorController()))
	{
		TargetLocation = ACEnemy_AIController_Boss::TargetLocation;
	}

	ACEnemy* ControllingPawn = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return;


	FVector location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocation);
	EPathFollowingRequestResult::Type result = Cast<AAIController>(ControllingPawn->GetController())->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		if (ControllingPawn->GetPatrolPath())
			ControllingPawn->GetPatrolPath()->UpdateIndex();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;
	}
}



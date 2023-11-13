#include "AI/Task/CBT_SideMoving.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CEnemy.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "NavigationSystem.h"
#include "Character/CEnemy_Range.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Bow.h"


UCBT_SideMoving::UCBT_SideMoving()
{
	bNotifyTick = true;
	NodeName = TEXT("SideMoving");
}

EBTNodeResult::Type UCBT_SideMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllingPawn = Cast<AHumanType>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;


	randomIndex = FMath::RandRange(0, 1);


	timer = 0.0f;
	timerlimit = FMath::RandRange(1.5f, 4.5f);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBT_SideMoving::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllingPawn->IsBlocking = false;

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBT_SideMoving::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	//if(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(ACEnemy_AIController::Behavior) != (uint8)EAI_StateType::Alert)
	//{
	//	ControllingPawn->GetMovement()->SetSpeed(ESpeedType::Run);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//}



	Cast<ACEnemy>(ControllingPawn)->LookAtTarget();

	if(ControllingPawn->GetMovement()->CanMove())
	{
		ControllingPawn->IsBlocking = true;

		ControllingPawn->GetMovement()->SetSpeed(ESpeedType::Walk);


		if (randomIndex == 1)
		{
			ControllingPawn->AddMovementInput(ControllingPawn->GetActorRightVector(), 1);
		}
		else
		{
			ControllingPawn->AddMovementInput(ControllingPawn->GetActorRightVector(), -1);
		}



		if(Cast<UCDoAction_Bow>(Cast<ACEnemy>(ControllingPawn)->GetWeapons()->GetDoAction()))
		{
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), Cast<ACEnemy>(ControllingPawn)->GetTarget()->GetActorLocation());

			Cast<ACEnemy_Range>(ControllingPawn)->pitchRange = Rotator.Pitch;

			Cast<ACEnemy>(ControllingPawn)->AI_Attack();
		}
	}

	if(timer >= timerlimit)
	{
		ControllingPawn->IsBlocking = false;
		ControllingPawn->GetMovement()->SetSpeed(ESpeedType::Run);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		timer += DeltaSeconds;
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}

}

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBT_Task_MoveToLocation.generated.h"

UCLASS()
class MYPROJECT_06_API UCBT_Task_MoveToLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBT_Task_MoveToLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FName TargetLocationName = NAME_None;
	FName BehaviorName= NAME_None;
	FName TargetName= NAME_None;
};

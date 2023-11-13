#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBT_SideMoving.generated.h"

UCLASS()
class MYPROJECT_06_API UCBT_SideMoving : public UBTTaskNode
{
	GENERATED_BODY()

private:

	UCBT_SideMoving();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AHumanType* ControllingPawn = nullptr;

private:
	int randomIndex = 0;
	float timer = 0.0f;
	float timerlimit = 0.0f;

};

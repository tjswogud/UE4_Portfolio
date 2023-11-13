#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Component/CStateComponent.h"
#include "CBT_StateTask.generated.h"

UCLASS()
class MYPROJECT_06_API UCBT_StateTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "State")
		EStateType State = EStateType::Max;


public:
	UCBT_StateTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	class AHumanType* ControllingPawn = nullptr;

	FName BehaviorName = NAME_None;
	
};

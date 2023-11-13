#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBT_Task_Patrol.generated.h"


UCLASS()
class MYPROJECT_06_API UCBT_Task_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AcceptanceDistance = 20;

	UPROPERTY(EditAnywhere, Category = "Random")
		float RandomRadius = 1500;
public:
	UCBT_Task_Patrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

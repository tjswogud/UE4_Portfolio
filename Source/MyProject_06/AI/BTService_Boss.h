#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Boss.generated.h"

UCLASS()
class MYPROJECT_06_API UBTService_Boss : public UBTService
{
	GENERATED_BODY()


public:
	UBTService_Boss();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	float ActionRange = 500.0f;
	float AlertRange = 1700.0f;
};

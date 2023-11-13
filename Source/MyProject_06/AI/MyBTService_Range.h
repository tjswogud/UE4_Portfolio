#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MyBTService_Range.generated.h"

UCLASS()
class MYPROJECT_06_API UMyBTService_Range : public UBTService
{
	GENERATED_BODY()

public:
	UMyBTService_Range();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	float ActionRange = 1000.0f;
	float AlertRange = 2000.0f;


};

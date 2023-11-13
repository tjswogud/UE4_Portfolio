#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Enemy.generated.h"

UCLASS()
class MYPROJECT_06_API UCBTService_Enemy : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Enemy();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
private:
	float ActionRange = 300.0f;
	float AlertRange = 1700.0f;

};

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Character/CEnemy.h"
#include "CBT_PatternandAction.generated.h"

UCLASS()
class MYPROJECT_06_API UCBT_PatternandAction : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY()
		EAI_EnemyActionType ActionType = EAI_EnemyActionType::Max;

	UPROPERTY()
		EAI_StateType StateType = EAI_StateType::Max;

	UPROPERTY(EditAnywhere)
		EWeaponType Weapon = EWeaponType::Max;

public:
	UCBT_PatternandAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	ACEnemy* ControllingPawn = nullptr;

	FName BehaviorName = NAME_None;
	FName TargetName = NAME_None;

};

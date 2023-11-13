#pragma once

#include "CoreMinimal.h"
#include "Character/HumanType.h"
#include "AI/CEnemy_AIController.h"
#include "Components/WidgetComponent.h"
#include "CEnemy.generated.h"

UENUM(BlueprintType)
enum class EAI_StateType : uint8
{
	Hitted UMETA(DisplayName = "Hitted"),
	Patrol UMETA(DisplayName = "Patrol"),
	Alert	UMETA(DisplayName = "Alert"),
	Avoid	UMETA(DisplayName = "Avoid"),
	Approach UMETA(DisplayName = "Approach"),
	Action	UMETA(DisplayName = "Action"),
	Groggy	UMETA(DisplayName = "Groggy"),
	Execute	UMETA(DisplayName = "Execute"),
	Dead	UMETA(DisplayName = "Dead"),
	Parried	UMETA(DisplayName = "Parried"),
	Ground	UMETA(DisplayName = "Ground"),
	RopeAction	UMETA(DisplayName = "RopeAction"),
	Max		UMETA(DisplayName = "Max")
};





UENUM()
enum class EAI_EnemyActionType : uint8
{
	Attack, Block, Skill, Avoid, Max
};

UENUM()
enum class EDistancefromTarget : uint8
{
	Near, Middle, Far, Max
};


USTRUCT(BlueprintType)
struct FActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EAI_EnemyActionType ActionType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		int Index = 0;
};



UCLASS()
class MYPROJECT_06_API ACEnemy : public AHumanType
{
	GENERATED_BODY()



public:
	UPROPERTY(BlueprintReadOnly)
		EAI_StateType AIState = EAI_StateType::Max;

public:
	UPROPERTY(EditAnywhere, Category = "EnemySettings")
		int EnemyType = 0;

	UPROPERTY(EditAnywhere, Category = "EnemySettings")
		EDistancefromTarget DistancefromTarget = EDistancefromTarget::Max;

public:
	UPROPERTY()
		EAI_EnemyActionType ActionType = EAI_EnemyActionType::Max;

	UPROPERTY()
		EAI_StateType PatternType = EAI_StateType::Max;

public:
	UPROPERTY(EditAnywhere, Category = "StatePatterns")
		TArray<EAI_StateType> FarPatterns;

	UPROPERTY(EditAnywhere, Category = "StatePatterns")
		TArray<EAI_StateType> MiddlePatterns;

	UPROPERTY(EditAnywhere, Category = "StatePatterns")
		TArray<EAI_StateType> NearPatterns;

	UPROPERTY(EditAnywhere, Category = "ActionPatterns")
		TArray<EAI_EnemyActionType> FarActions;

	UPROPERTY(EditAnywhere, Category = "ActionPatterns")
		TArray<EAI_EnemyActionType> MiddleActions;

	UPROPERTY(EditAnywhere, Category = "ActionPatterns")
		TArray<EAI_EnemyActionType> NearActions;

	UPROPERTY(EditAnywhere, Category = "Skills")
		TArray<int> NearSkills;

	UPROPERTY(EditAnywhere, Category = "Skills")
		TArray<int> MiddleSkills;

	UPROPERTY(EditAnywhere, Category = "Skills")
		TArray<int> FarSkills;



	UPROPERTY(EditAnywhere, Category = "Targetting")
		UUserWidget* Widget;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bReverse = false;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UWidgetComponent* TargetWidget;

public:
	ACEnemy();


	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolData; }




protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

protected:
	virtual void Hitted() override;

public:
	virtual void End_Hitted() override;

protected:
	virtual void Dead() override;

public:
	virtual void End_Dead() override;

public:
	virtual void AI_Attack();
	virtual void AI_Block();
	virtual void AI_Skill();
	virtual void AI_Avoid();

	virtual void AI_RopeAction();


public:
	void LookAtTarget();
	void SetPrevState(EAI_StateType InPrev);

	AHumanType* GetTarget();

	template< class T >
	T GetRandomIndexData(TArray<T> Inarray);

	UBlackboardComponent* GetBlackboardComponent(); 

public:
	EAI_StateType InPrevState;

	UPROPERTY()
		ACPatrolPath* PatrolData;
};


#include "AI/CEnemy_AIController_Range.h"
#include "Global.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/HumanType.h"


const FName ACEnemy_AIController_Range::Target(TEXT("Target"));
const FName ACEnemy_AIController_Range::Behavior(TEXT("Behavior"));
const FName ACEnemy_AIController_Range::EQS_Location(TEXT("EQS_Location"));
const FName ACEnemy_AIController_Range::TargetLocation(TEXT("TargetLocation"));

ACEnemy_AIController_Range::ACEnemy_AIController_Range()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Enemies/EnemyAI/BB_EnemyAI.BB_EnemyAI'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Enemies/EnemyAI/BT_AIRange.BT_AIRange'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}


	SetPerceptionSystem();
}

void ACEnemy_AIController_Range::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void ACEnemy_AIController_Range::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}
}

void ACEnemy_AIController_Range::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent) return;

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void ACEnemy_AIController_Range::OnUpdated(TArray<AActor*> const& updated_actors)
{
}

void ACEnemy_AIController_Range::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (Cast<AHumanType>(actor) && !GetBlackboardComponent()->GetValueAsObject("Target"))
	{
		auto const target = Cast<AHumanType>(actor);
		if (Cast<ACPlayer>(target))
		{
			GetBlackboardComponent()->SetValueAsObject("Target", target);
		}
		else if (Cast<ACEnemy>(target)->EnemyType == Cast<ACEnemy>(this->GetPawn())->EnemyType)
		{

		}
		else
		{
			GetBlackboardComponent()->SetValueAsObject("Target", target);
		}
	}
}

void ACEnemy_AIController_Range::SetPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));


	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACEnemy_AIController_Range::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

}

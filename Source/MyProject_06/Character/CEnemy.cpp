#include "Character/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "CEnemy_Boss.h"
#include "CEnemy_Melee.h"
#include "CEnemy_Range.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "AI/CEnemy_AIController_Range.h"
#include "AI/CPatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy::ACEnemy() : AHumanType()
{
	
	
}



void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	//if (UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this->GetWorld(), TargetWidget->GetWidgetClass()))
	//{
	//	TargetWidget->SetWidget(WidgetInstance);

	//}



}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

}

void ACEnemy::Hitted()
{
	Super::Hitted();

	
}

void ACEnemy::End_Hitted()
{
	if(Status->CheckGroggy())
	{
		State->SetGroggyMode();
	}
	else
	{
		State->SetIdleMode();
	}

}

void ACEnemy::Dead()
{
	Super::Dead();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::AI_Attack()
{
}

void ACEnemy::AI_Block()
{
}

void ACEnemy::AI_Skill()
{
}

void ACEnemy::AI_Avoid()
{
}

void ACEnemy::AI_RopeAction()
{
}

void ACEnemy::LookAtTarget()
{
	AHumanType* target = nullptr;


	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(GetActorLocation()), CHelpers::GetVector2D(target->GetActorLocation()));
	SetActorRotation(rotator);



}

void ACEnemy::SetPrevState(EAI_StateType InPrev)
{
	if (InPrevState == InPrev)
	{
		return;
	}
	else if (InPrev == EAI_StateType::Patrol)
		return;

	InPrevState = InPrev;
}

AHumanType* ACEnemy::GetTarget()
{
	AHumanType* Target = Cast<AHumanType>(Cast<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject("Target"));

	CheckNullResult(Target, nullptr);

	return Target;
}

UBlackboardComponent* ACEnemy::GetBlackboardComponent()
{

	CheckNullResult(Cast<AAIController>(GetController())->GetBlackboardComponent(), nullptr)

	return Cast<AAIController>(GetController())->GetBlackboardComponent();
}

template <class T>
T ACEnemy::GetRandomIndexData(TArray<T> Inarray)
{
	FAILED(Inarray.Num() > 0);


	return Inarray[FMath::RandRange(0, Inarray.Num() - 1)];
}


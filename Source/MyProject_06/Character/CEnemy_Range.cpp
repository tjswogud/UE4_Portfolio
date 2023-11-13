#include "Character/CEnemy_Range.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "AI/CEnemy_AIController_Range.h"
#include "Components/CapsuleComponent.h"
#include "Component/CWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

ACEnemy_Range::ACEnemy_Range()
{
	AIControllerClass = ACEnemy_AIController_Range::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PrimaryActorTick.bCanEverTick = true;



	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &TargetWidget, "TargetWidget", this->GetCapsuleComponent(), "TargetFocus");
	TargetWidget->SetVisibility(false);
	HealthBar->SetVisibility(false);


	HealthBar->SetRelativeLocation(FVector(0, 0, 130));
}

void ACEnemy_Range::BeginPlay()
{
	Super::BeginPlay();


	Cast<ACEnemy_AIController_Range>(GetController())->GetBlackboardComponent()->SetValueAsEnum(ACEnemy_AIController_Range::Behavior, (uint8)EAI_StateType::Patrol);



	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Range::OnStateTypeChanged);

}

void ACEnemy_Range::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);


}

void ACEnemy_Range::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	

	TargetWidget->SetWorldLocation(this->GetMesh()->GetSocketLocation("Chest"));


	if ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - this->GetActorLocation()).Size() < 3000)
	{
		HealthBar->SetVisibility(true);
	}
	else
	{
		HealthBar->SetVisibility(false);
	}
}

void ACEnemy_Range::Hitted()
{
	Super::Hitted();

	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		if (Status->IsDead() == false)
		{
			FVector direction;
			FVector start;
			FVector target;

			if (Damage.Event->HitData->HitLocation.Equals(FVector::ZeroVector))
			{
				target = Damage.Character->GetActorLocation();
			}
			else
			{
				target = Damage.Event->HitData->HitLocation;
			}
			start = GetActorLocation();

			direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(start), CHelpers::GetVector2D(target)));
		}


		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());

		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		return;
	}




	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy_Range::End_Hitted()
{
	Super::End_Hitted();
}

void ACEnemy_Range::Dead()
{
	Super::Dead();
}

void ACEnemy_Range::End_Dead()
{
	Super::End_Dead();
}

float ACEnemy_Range::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACEnemy_Range::AI_Attack()
{
	if (!!Weapons->GetDoAction())
	{
		Cast<UCDoAction_Bow>(Weapons->GetDoAction())->DoAction();
	}
}

void ACEnemy_Range::AI_Avoid()
{
	Super::AI_Avoid();
}

void ACEnemy_Range::AI_Skill()
{
	Super::AI_Skill();
}

void ACEnemy_Range::Equip()
{
	Weapons->SetBowMode();
}

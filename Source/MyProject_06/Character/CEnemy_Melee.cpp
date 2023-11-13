#include "Character/CEnemy_Melee.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CMovementComponent.h"
#include "AI/CEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CStatusComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy_Melee::ACEnemy_Melee() : ACEnemy()
{
	AIControllerClass = ACEnemy_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PrimaryActorTick.bCanEverTick = true;

	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &TargetWidget, "TargetWidget", this->GetCapsuleComponent(), "TargetFocus");
	TargetWidget->SetVisibility(false);
	HealthBar->SetVisibility(false);

	HealthBar->SetRelativeLocation(FVector(0, 0, 130));
}

void ACEnemy_Melee::BeginPlay()
{
	Super::BeginPlay();

	
	Cast<ACEnemy_AIController>(GetController())->GetBlackboardComponent()->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Max);
	

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Melee::OnStateTypeChanged);





	
}

void ACEnemy_Melee::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	
}

void ACEnemy_Melee::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	

	TargetWidget->SetWorldLocation(this->GetMesh()->GetSocketLocation("Chest"));

	if((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - this->GetActorLocation()).Size() < 3000 )
	{
		HealthBar->SetVisibility(true);
	}
	else
	{
		HealthBar->SetVisibility(false);
	}
}


void ACEnemy_Melee::Hitted()
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

			if(Damage.Event->HitData->HitLocation.Equals(FVector::ZeroVector))
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


	}




	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy_Melee::End_Hitted()
{
	Super::End_Hitted();
}

void ACEnemy_Melee::Dead()
{
	Super::Dead();
}

void ACEnemy_Melee::End_Dead()
{
	Super::End_Dead();
}

float ACEnemy_Melee::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACEnemy_Melee::AI_Attack()
{
	if(!!Weapons->GetDoAction())
	{
		Weapons->DoAction();
	}
}

void ACEnemy_Melee::AI_Avoid()
{
	Super::AI_Avoid();
}

void ACEnemy_Melee::AI_Block()
{
	Super::AI_Block();
}

void ACEnemy_Melee::AI_Skill()
{
	Super::AI_Skill();
}

void ACEnemy_Melee::Equip()
{
	Weapons->SetSwordMode();
}


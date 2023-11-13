#include "Character/CEnemy_Boss.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CMovementComponent.h"
#include "AI/CEnemy_AIController.h"
#include "AI/CEnemy_AIController_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CRopeActionComponent.h"
#include "Component/CStatusComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;



	CHelpers::CreateActorComponent<UCRopeActionComponent>(this, &RopeActionComponent, "RopeAction");




	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &TargetWidget, "TargetWidget", this->GetMesh(), "Aim_Target");
	TargetWidget->SetVisibility(false);
	HealthBar->SetVisibility(false);
}

void ACEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();

	
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Boss::OnStateTypeChanged);

	this->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemy_Boss::OnHit);
	


	Weapons->SetSwordMode();
}

void ACEnemy_Boss::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);



}

void ACEnemy_Boss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	
	TargetWidget->SetWorldLocation(this->GetMesh()->GetSocketLocation("Chest"));


}

void ACEnemy_Boss::Hitted()
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


	}

	if (Status->IsDead())
	{
		State->SetDeadMode();

		return;
	}




	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy_Boss::End_Hitted()
{
	Super::End_Hitted();
}

void ACEnemy_Boss::Dead()
{
	Super::Dead();
}

void ACEnemy_Boss::End_Dead()
{
	Super::End_Dead();
}

void ACEnemy_Boss::Landed(const FHitResult& Hit)
{
	

	Super::Landed(Hit);

	

}

float ACEnemy_Boss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACEnemy_Boss::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit)
	{
		UAnimMontage* CurrentMontage = this->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();

	
		
	}

}

void ACEnemy_Boss::AI_Attack()
{
	Super::AI_Attack();

	if (!!Weapons->GetDoAction())
	{
		Cast<UCDoAction>(Weapons->GetDoAction())->DoAction();
	}
}

void ACEnemy_Boss::AI_Avoid()
{
	Super::AI_Avoid();
}

void ACEnemy_Boss::AI_Block()
{
	Super::AI_Block();
}

void ACEnemy_Boss::AI_Skill()
{
	Super::AI_Skill();
}



void ACEnemy_Boss::Equip()
{
	Super::Equip();

	Weapons->SetSwordMode();
}


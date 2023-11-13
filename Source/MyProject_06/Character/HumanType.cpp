#include "Character/HumanType.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"

AHumanType::AHumanType()
{
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapons, "Weapons");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthBar, "HealthBar", GetCapsuleComponent());

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	
}

void AHumanType::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{

	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	case EStateType::Ground: Ground(); break;
	case EStateType::Rise: Rise(); break;
	case EStateType::Groggy: Groggy(); break;
	case EStateType::Execute: Execute(); break;
	case EStateType::Idle: Idle(); break;
	case EStateType::Dodge: break;
	case EStateType::Equip: Equip();	break;
	case EStateType::Action: break;
	case EStateType::Sneak: break;
	case EStateType::Hide: break;
	case EStateType::Parried: break;
	case EStateType::Max: break;
	default:;
	}
}

void AHumanType::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (Status->GetPlaceType()) {
	case EPlaceType::OnGround: break;
	case EPlaceType::OnLand: 
		
		break;
	case EPlaceType::OnWall: 
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		break;
	case EPlaceType::InAir:   	
		break;
	default: ;
	}
}

float AHumanType::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<AHumanType>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	if(Status->CheckGroggy())
	{
		Hitted();
		return 0.0f;
	}
	else if(State->IsExecuteMode())
	{
		return 0.0f;
	}
	else if(IsBlocking)
	{
		if(Damage.Event->HitData->HittedType == EHittedType::StrongHit)
		{
			Hitted();

			return 0.0f;
		}

		float dot = FVector::DotProduct(this->GetActorForwardVector(), Damage.Character->GetActorForwardVector());
		if(dot <= -0.2)
		{
			if(CanParry)
			{
				Damage.Character->GetMontages()->PlayRandomIndexOfAnim(EStateType::Parried);
				Damage.Character->GetStatus()->CurGroggy += 20;
				
				if (Damage.Character->GetStatus()->IsGroggy() && !Damage.Character->GetStatus()->CheckGroggy())
				{

					Damage.Character->GetStatus()->ToggleGroggy();
					Damage.Character->GetState()->SetGroggyMode();
				}
			}

			if(Weapons->GetCurWeapon()->GetDeflectActionDatas().Num() <= 0)
			{
				return 0.0f;
			}


			FDoActionData DeflectAction = Weapons->GetCurWeapon()->GetDeflectActionDatas()[FMath::RandRange(0, Weapons->GetCurWeapon()->GetDeflectActionDatas().Num()-1)];

			DeflectAction.DoAction(this);

			
			return 0.0f;
		}
		else
		{
			State->SetHittedMode();
		}

	}
	else
	{
		switch (ActionState) {
		case EVariousActionState::UnHitable: 

			return 0.0f;
			break;
		case EVariousActionState::UnStopable: 
			Hitted();

			return 0.0f;
			break;
		case EVariousActionState::Max: 
			State->SetHittedMode();

			break;
		default: ;
		}

		
	}


	return damage;
}

void AHumanType::UseSkill(int InIndex)
{
	CheckTrue(Weapons->IsUnarmedMode())
	CheckTrue(Weapons->GetCurWeapon()->GetDoAction()->GetBeginAction())

	CheckNull(Weapons->GetCurWeapon());
	CheckNull(Weapons->GetCurWeapon()->GetDoAction());

	State->SetActionMode();

	CheckNull(Weapons);
	Weapons->GetCurWeapon()->GetDoAction()->Begin_DoAction();

	Weapons->GetDoAction()->InUseSkill = true;

	if(InIndex > Weapons->GetCurWeapon()->SkillActionDatas.Num() - 1)
	{
		CLog::Print("Wrong Skill Index");
		Weapons->SkillAction(0);
	}
	else
	{
		Weapons->SkillAction(InIndex);
	}


	
}

void AHumanType::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);


	if(Hit.bBlockingHit)
	{
		this->SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));

		if (Status->GetPlaceType() == EPlaceType::InAir && (State->IsJumpMode() || State->IsIdleMode()))
		{

			Montages->PlayAnimMontage(EStateType::Jump, 0, "Land");
			
		}
		else if(Status->GetPlaceType() == EPlaceType::InAir && State->IsActionMode())
		{


			
		}
		else if (Status->GetPlaceType() == EPlaceType::InAir && State->IsHideMode())
		{

		}

		Status->SetPlaceType(EPlaceType::OnLand);

	}

	
}

void AHumanType::HiddenHealthBar(bool IsHidden)
{
	if (!IsValid(HealthBar->GetWidget())) return;

	if(IsHidden)
	{
		HealthBar->GetWidget()->RemoveFromViewport();
	}
	else
	{
		HealthBar->GetWidget()->AddToViewport();
	}
}

void AHumanType::Idle()
{
	

	SetEndState();
}

void AHumanType::End_Ground()
{
	SetEndState();
}

void AHumanType::End_Rise()
{
	State->SetIdleMode();


}

void AHumanType::BeginPlay()
{
	Super::BeginPlay();

	Movement->SetSpeed(ESpeedType::Run);
	Movement->DisableControlRotation();

	//State->OnStateTypeChanged.AddDynamic(this, &AHumanType::OnStateTypeChanged);
}

void AHumanType::SetEndState()
{



	Movement->SetSpeed(ESpeedType::Run);
	Movement->Move();
	Movement->EnableControlRotation();
	Movement->DisableFixedCamera();

	CheckNull(Weapons)
	if(!Weapons->IsUnarmedMode())
	{
		CheckNull(Weapons->GetCurWeapon());
		CheckNull(Weapons->GetCurWeapon()->GetDoAction());

		UCDoAction_Combo* UcDoAction_Combo = (UCDoAction_Combo*)Weapons->GetCurWeapon()->GetDoAction();

		CheckNull(UcDoAction_Combo);
		UcDoAction_Combo->DisableCombo();
		UcDoAction_Combo->GetExist() = false;
		UcDoAction_Combo->GetComboIndex() = 0;

	}



}

void AHumanType::Hitted()
{
	Status->Damage(Damage.Power, Damage.GroggyRate);
	//Damage.Power = 0;


}

void AHumanType::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName("Execute");
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();

	HealthBar->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]() {
		// 딜레이 후 실행할 코드 작성
		for (UCWeaponData* data : Weapons->Datas)
		{
			if(IsValid(data))
			{
				data->GetAttachment()->Destroy();
			}
		}
		this->Destroy();

	}, 10.0f, false);

}

void AHumanType::Dodge()
{


	Movement->Stop();
	Movement->EnableControlRotation();
	Montages->PlayDodgeMode();
}

void AHumanType::Ground()
{
	Movement->Stop();
	Movement->DisableControlRotation();

	Montages->PlayGroundMode();
}

void AHumanType::Rise()
{
	Movement->Stop();

	Montages->PlayRiseMode();
}

void AHumanType::Groggy()
{
	Movement->Stop();
}

void AHumanType::Execute()
{


	Movement->Stop();
	Movement->DisableControlRotation();

	if(Cast<ACEnemy_AIController>(GetController()))
	{
		
		Cast<ACEnemy_AIController>(GetController())->GetBlackboardComponent()->SetValueAsEnum(ACEnemy_AIController::Behavior, (uint8)EAI_StateType::Execute);
	}
}

void AHumanType::Parried()
{
	Movement->Stop();
	Movement->DisableControlRotation();
}

void AHumanType::Equip()
{
	
}

void AHumanType::Jump()
{
	if (State->CurJumpCount >= State->MaxJumpCount)
	{
		return;
	}

	GetCharacterMovement()->JumpOff(this);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetStatus()->SetPlaceType(EPlaceType::InAir);
	Montages->PlayAnimMontage(EStateType::Jump, State->CurJumpCount);
	State->CurJumpCount++;
}

void AHumanType::End_Dodge()
{


	State->SetIdleMode();
}

void AHumanType::End_Hitted()
{
	if(!Weapons->IsUnarmedMode())
	{
		Weapons->GetDoAction()->End_DoAction();
	}
}

void AHumanType::End_Parried()
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

void AHumanType::End_Jump()
{
	State->CurJumpCount = 0;

	State->SetIdleMode();
}

void AHumanType::End_Groggy()
{
	SetEndState();


	if(Status->CheckGroggy())
	{
		Status->ToggleGroggy();

	}
}

void AHumanType::End_Execute()
{
	State->SetIdleMode();
}

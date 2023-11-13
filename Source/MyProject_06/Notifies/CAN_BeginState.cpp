#include "Notifies/CAN_BeginState.h"
#include "Global.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CPlayer.h"
#include "Character/HumanType.h"
#include "Component/CRopeActionComponent.h"

FString UCAN_BeginState::GetNotifyName_Implementation() const
{
	return "BeginState";
}

void UCAN_BeginState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AHumanType* character = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(character);



	switch (StateType) {
	case EStateType::Idle:	character->GetState()->SetIdleMode(); break;
	case EStateType::Dodge: character->GetState()->SetDodgeMode();	break;
	case EStateType::Equip: character->GetState()->SetEquipMode();	break;
	case EStateType::Hitted:	character->GetState()->SetHittedMode();	break;
	case EStateType::Dead:	character->GetState()->SetDeadMode(); break;
	case EStateType::Action:	character->GetState()->SetActionMode();	break;
	case EStateType::Ground:


		character->GetState()->IsFront = IsFront;
		character->GetState()->SetGroundMode();
		break;
	case EStateType::Rise:	character->GetState()->SetRiseMode();	break;
	case EStateType::Max:	CLog::Print("MAX");	break;
	case EStateType::Sneak: break;
	case EStateType::Hide: break;
	case EStateType::Groggy: break;
	case EStateType::Execute: break;
	case EStateType::Parried: break;
	case EStateType::Jump: break;
	case EStateType::RopeAction:
		if(Cast<ACPlayer>(character))
		{
			Cast<ACPlayer>(character)->RopeActionComponent->ThrowHook();
		}
		else if(Cast<ACEnemy_Boss>(character))
		{
			Cast<ACEnemy_Boss>(character)->RopeActionComponent->ThrowHook();
		}
		

		break;
	default: ;
	}
}

#include "Notifies/CAN_EndState.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/HumanType.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
#include "Components/CapsuleComponent.h"

FString UCAN_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAN_EndState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AHumanType* character = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(character);



	switch (StateType)
	{
		case EStateType::Dodge: character->End_Dodge(); break;
		case EStateType::Hitted: character->End_Hitted(); break;
		case EStateType::Dead: character->End_Dead(); break;
		case EStateType::Idle: break;
		case EStateType::Equip: break;
		case EStateType::Action: break;
		case EStateType::Ground: break;
		case EStateType::Rise:	character->End_Rise(); break;
		case EStateType::Groggy: character->End_Groggy(); break;
		case EStateType::Execute: 
			character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
			
			character->End_Execute(); break;
		case EStateType::Max: break;
		case EStateType::Sneak: break;
		case EStateType::Hide: break;
		case EStateType::Parried: character->End_Parried();	break;
		case EStateType::Jump: character->End_Jump();	break;
		case EStateType::RopeAction: 
			Cast<ACPlayer>(character)->End_RopeAction();

		break;
		default: ;
	}
}
#include "Weapons/CSubAction.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Components/CapsuleComponent.h"


UCSubAction::UCSubAction()
{
}

void UCSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner = Cast<AHumanType>(InOwner);
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCSubAction::Pressed()
{
	bInAction = true;
}

void UCSubAction::Released()
{
	bInAction = false;
}
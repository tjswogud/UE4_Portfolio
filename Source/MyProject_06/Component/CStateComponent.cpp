#include "CStateComponent.h"

#include "CStatusComponent.h"
#include "Character/HumanType.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetDodgeMode()
{
	ChangeType(EStateType::Dodge);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetGroundMode()
{
	ChangeType(EStateType::Ground);

}

void UCStateComponent::SetRiseMode()
{
	ChangeType(EStateType::Rise);
}

void UCStateComponent::SetSneakMode()
{
	ChangeType(EStateType::Sneak);
}

void UCStateComponent::SetHideMode()
{
	ChangeType(EStateType::Hide);
}

void UCStateComponent::SetGroggyMode()
{
	ChangeType(EStateType::Groggy);
}

void UCStateComponent::SetExecuteMode()
{
	ChangeType(EStateType::Execute);
}

void UCStateComponent::SetParriedMode()
{
	ChangeType(EStateType::Execute);
}

void UCStateComponent::SetJumpMode()
{
	if (IsClimbMode()) return;


	ChangeType(EStateType::Jump);
}

void UCStateComponent::SetRopeActionMode()
{
	ChangeType(EStateType::RopeAction);
}

void UCStateComponent::SetClimbMode()
{
	ChangeType(EStateType::Climb);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

void UCStateComponent::ChangeType(EStateType InType)
{

	PrevType = Type;

	Type = InType;


	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(PrevType, Type);
}

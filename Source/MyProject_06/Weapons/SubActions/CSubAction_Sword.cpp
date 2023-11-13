#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Component/CMontagesComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CMontagesComponent.h"

#include "Weapons/CWeaponAsset.h"

void UCSubAction_Sword::Pressed()
{

	Super::Pressed();

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());



	State->SetActionMode();
	State->OnSubActionMode();


	Owner->GetMovement()->SetSpeed(ESpeedType::Walk);
	Owner->IsBlocking = true;


	StartParry();

	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "EndParry");

	Owner->GetWorld()->GetTimerManager().SetTimer(End_ParryTimer, timerDelegate, 0.2f, false);


	//FDoActionData blockData = Owner->GetWeapons()->GetCurWeapon()->GetBlockData();
	//Owner->PlayAnimMontage(blockData.Montage, blockData.PlayRate);


}

void UCSubAction_Sword::Released()
{

	Super::Released();

	State->SetIdleMode();
	State->OffSubActionMode();



	Owner->GetMovement()->SetSpeed(ESpeedType::Run);
	Owner->IsBlocking = false;

	//FDoActionData blockData = Owner->GetWeapons()->GetCurWeapon()->GetBlockData();

	//Owner->PlayAnimMontage(blockData.Montage, blockData.PlayRate, "End");
}

void UCSubAction_Sword::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();
}

void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();
}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}


void UCSubAction_Sword::StartParry()
{
	Owner->CanParry = true;
}

void UCSubAction_Sword::EndParry()
{
	Owner->CanParry = false;
}

#include "Notifies/CAN_SetMovement.h"
#include "Global.h"
#include "Character/HumanType.h"

FString UCAN_SetMovement::GetNotifyName_Implementation() const
{
	return "SetMovement";
}

void UCAN_SetMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AHumanType* Owner = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(Owner);

	
	if(FixCamera)
	{
		Owner->GetMovement()->EnableFixedCamera();
	}
	else
	{
		Owner->GetMovement()->DisableFixedCamera();
	}


	if(EnableControlRotation)
	{
		Owner->GetMovement()->EnableControlRotation();
	}
	else
	{
		Owner->GetMovement()->DisableControlRotation();
	}

	if(CanMove)
	{
		Owner->GetMovement()->Move();
	}
	else
	{
		Owner->GetMovement()->Stop();
	}

	switch (MoveSpeed)
	{
	case ESpeedType::Sneak: Owner->GetMovement()->SetSpeed(ESpeedType::Sneak); break;
	case ESpeedType::Walk: Owner->GetMovement()->SetSpeed(ESpeedType::Walk); break;
	case ESpeedType::Run: Owner->GetMovement()->SetSpeed(ESpeedType::Run); break;
	case ESpeedType::Sprint: Owner->GetMovement()->SetSpeed(ESpeedType::Sprint); break;
	case ESpeedType::Max: break;
	default: ;
	}

}

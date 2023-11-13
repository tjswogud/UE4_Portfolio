#include "Notifies/CANS_VariousActionState.h"
#include "Character/HumanType.h"
#include "Global.h"

FString UCANS_VariousActionState::GetNotifyName_Implementation() const
{
	return "VariousState";
}

void UCANS_VariousActionState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AHumanType* Owner = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(Owner);


	Owner->ActionState = ActionState;

}

void UCANS_VariousActionState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AHumanType* Owner = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(Owner);


	Owner->ActionState = EVariousActionState::Max;

}

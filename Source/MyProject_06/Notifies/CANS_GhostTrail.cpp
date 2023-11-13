#include "Notifies/CANS_GhostTrail.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Component/CWeaponComponent.h"

FString UCANS_GhostTrail::GetNotifyName_Implementation() const
{
	return "GhostTrail";
}

void UCANS_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<AHumanType>(MeshComp->GetOwner());

	CheckNull(Owner);
	CheckNull(GhostTrailClass);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);


	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


}

void UCANS_GhostTrail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCANS_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	CheckNull(Owner);
	CheckNull(GhostTrailClass);
	CheckNull(GhostTrail);


	GhostTrail->Destroy();
}

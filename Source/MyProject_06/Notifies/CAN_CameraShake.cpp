#include "Notifies/CAN_CameraShake.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShake.h"

FString UCAN_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAN_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Character);

	APlayerController* controller = Character->GetController<APlayerController>();
	CheckNull(controller);


	/*CS = NewObject<UMatineeCameraShake>();

	CS->OscillationDuration = 0.25;
	CS->RotOscillation = RotOscillation;
	CS->LocOscillation = LocOscillation;
	CS->FOVOscillation = FOVOscillation;
	CS->AnimPlayRate= 1.0f;
	CS->AnimScale = 1.0f;
	CS->AnimBlendInTime = 0.2f;
	CS->AnimBlendOutTime = 0.2f;*/

	//CS->StartShake(controller->PlayerCameraManager, 1.0f, ECameraShakePlaySpace::CameraLocal);


	if(IsValid(CameraShake))
	{

		controller->PlayerCameraManager->StartCameraShake(CameraShake);
	}


	  
	

}

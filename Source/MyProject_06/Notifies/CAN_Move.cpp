#include "Notifies/CAN_Move.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CEnemy.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UCAN_Move::GetNotifyName_Implementation() const
{
	return "Move";
}

void UCAN_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<AHumanType>(MeshComp->GetOwner());
	CheckNull(Owner);

	GetDirectionVector(MoveDirection);

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentMontage)
		{
			CurrentMontage->RootMotionRootLock = ERootMotionRootLock::RefPose;
		}
	}



	if(bSetVelocityZero)
	{
		Owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}


	switch (MoveType) {
	case EMoveType::Velocity:
		Owner->GetCharacterMovement()->Velocity = Power * DirectVector;


		break;
	case EMoveType::Launch:


		Owner->LaunchCharacter(Power * DirectVector, IsOverride, IsOverride);
		//Owner->LaunchCharacter(Power * DirectVector, false, false);

		break;
	case EMoveType::Force:
		Owner->GetCharacterMovement()->AddForce(Power * DirectVector);

		break;
	case EMoveType::Impulse:
		Owner->GetCharacterMovement()->AddImpulse(Power * DirectVector);

		break;

	default:;
	}


		
}


void UCAN_Move::GetDirectionVector(EPowerDirection Direction)
{

	switch (Direction) {
	case EPowerDirection::Forward:
		DirectVector = Owner->GetActorForwardVector();
		break;
	case EPowerDirection::Backward:
		DirectVector = -1 * Owner->GetActorForwardVector();
		break;
	case EPowerDirection::Left:
		DirectVector = -1 * Owner->GetActorRightVector();
		break;
	case EPowerDirection::Right:
		DirectVector = Owner->GetActorRightVector();
		break;
	case EPowerDirection::Up:
		DirectVector = Owner->GetActorUpVector();
		break;
	case EPowerDirection::Down:
		DirectVector = -1 * Owner->GetActorUpVector();
		break;
	case EPowerDirection::ForwardDown: 
		DirectVector =  (-1 * Owner->GetActorUpVector() * ForwardUpRatio + Owner->GetActorForwardVector()).GetSafeNormal();

		break;
	case EPowerDirection::ForwardUp: 
		DirectVector = (Owner->GetActorUpVector() * ForwardUpRatio + Owner->GetActorForwardVector()).GetSafeNormal();

		break;
	case EPowerDirection::Random: 
		DirectVector = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1)).GetSafeNormal();


		break;
	default:;
	}

}

#include "CMovementComponent.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UCMovementComponent::UCMovementComponent()
{

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AHumanType>(GetOwner());
} 

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint()
{
	
	if(OwnerCharacter->GetState()->IsSneakMode())
	{
		DisableControlRotation();
		SetSpeed(ESpeedType::Walk);
	}
	else if(OwnerCharacter->IsBlocking)
	{
		SetSpeed(ESpeedType::Walk);
	}
	else
	{
		DisableControlRotation();
		SetSpeed(ESpeedType::Sprint);
	}
}

void UCMovementComponent::OnRun()
{

	if (OwnerCharacter->GetState()->IsSneakMode())
	{
		DisableControlRotation();
		SetSpeed(ESpeedType::Sneak);
	}
	else if (OwnerCharacter->IsBlocking)
	{
		EnableControlRotation();
		SetSpeed(ESpeedType::Walk);
	}
	else
	{
		EnableControlRotation();
		SetSpeed(ESpeedType::Run);
	}
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnSneak()
{

	if(OwnerCharacter->GetState()->IsSneakMode())
	{
		EnableControlRotation();
		SetSpeed(ESpeedType::Run);
		Cast<ACPlayer>(OwnerCharacter)->GetState()->SetIdleMode();
	}
	else if(OwnerCharacter->GetState()->IsIdleMode())
	{
		DisableControlRotation();
		SetSpeed(ESpeedType::Sneak);
		Cast<ACPlayer>(OwnerCharacter)->Sneak();
	}
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	if (InAxis > 0.0f)
	{
		OwnerCharacter->Direction = EMoveDirection::Forward;
	}
	else if (InAxis < 0.0f)
	{
		OwnerCharacter->Direction = EMoveDirection::Behind;
	}

	MoveForwardValue = InAxis;

	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);


}

void UCMovementComponent::OnMoveRight(float InAxis)
{


	if (InAxis > 0.0f)
	{
		OwnerCharacter->Direction = EMoveDirection::Right;
	}
	else if (InAxis < 0.0f)
	{
		OwnerCharacter->Direction = EMoveDirection::Left;
	}

	MoveRightValue = InAxis;

	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);

}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}
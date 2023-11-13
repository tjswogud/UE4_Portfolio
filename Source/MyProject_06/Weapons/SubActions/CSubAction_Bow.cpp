#include "Weapons/SubActions/CSubAction_Bow.h"

#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/HumanType.h"
#include "Character/CPlayer.h"
#include "Component/CStateComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	Camera->SetMobility(EComponentMobility::Movable);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	Bow_Attachment = Cast<ACAttachment_Bow>(InAttachment);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!bow)
		Bend = bow->GetBend();


}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);

	TArray<AActor*> ignores;
	ignores.Add(Owner);
	FHitResult Result;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	if(GetInAction())
	{
		if (
			UKismetSystemLibrary::LineTraceSingleForObjects(Owner->GetWorld(), Cast<ACPlayer>(Owner)->CameraLocation + UKismetMathLibrary::GetForwardVector(Cast<ACPlayer>(Owner)->CameraRotator) * 500.0f, Cast<ACPlayer>(Owner)->CameraLocation + UKismetMathLibrary::GetForwardVector(Cast<ACPlayer>(Owner)->CameraRotator) * 3000.0f,
				objects, false, ignores, EDrawDebugTrace::ForOneFrame, Result, true))
		{
			IsblockingHit = true;
			Bow_Attachment->UserWidget->SetColorAndOpacity(FLinearColor::Red);
			LineEnd = Result.ImpactPoint;
		}
		else
		{
			IsblockingHit = false;
			Bow_Attachment->UserWidget->SetColorAndOpacity(FLinearColor::White);
		}
	}
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bend)
		*Bend = Output.Y;


}
void UCSubAction_Bow::Pressed()
{
	CheckTrue(State->IsSubActionMode());
	CheckNull(SpringArm);
	CheckNull(Camera);

	if (DoAction->InUseSkill || DoAction->InUseSkillCombo) return;

	Super::Pressed();

	State->OnSubActionMode();

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Bow_Attachment->UserWidget->SetVisibility(ESlateVisibility::Visible);


	Timeline.PlayFromStart();

	Bow_Attachment->StringHandAttach = true;
}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());
	CheckNull(SpringArm);
	CheckNull(Camera);


	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Bow_Attachment->UserWidget->SetVisibility(ESlateVisibility::Hidden);

	Timeline.ReverseFromEnd();

	Bow_Attachment->StringHandAttach = true;
}
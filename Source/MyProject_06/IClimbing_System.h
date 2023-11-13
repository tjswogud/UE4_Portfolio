#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IClimbing_System.generated.h"


UINTERFACE(MinimalAPI)
class UClimbing_System : public UInterface
{
	GENERATED_BODY()
};

class MYPROJECT_06_API IClimbing_System 
{

	GENERATED_BODY()

public:
	virtual void Enable_Climbing_System(bool InIsEnableClimbing);
	virtual void Input(float InMoveRight, float InMoveForward);
	virtual void States(bool InIsGrabLedge, bool InIsHanging);
	virtual void Reach(bool InCan_Reach);
	virtual void Jump_From(float InX, float InY, float InAngle);
	virtual void Freehang_Turn(bool InFreehang_Turn_Enable);
	virtual void Ledge_Climb_Up(bool IsLedge_Climb_Up);
	virtual void Climb_To_Ledge(float InAngle, bool InIsClimb, float InDistance, bool InIsShort_Climb, float InSide);
	virtual void Climb_Down_To_Ledge(bool InIsClimb_Down_Ledge);
	virtual void Enable_Freehang(bool InIsFreeHanging);
	virtual void Enable_Climb_Montage(bool InIsEnable_Climb_Montage_IK);
	virtual void Current_Hand_IK(FVector InHand_R, FVector InHand_L);
	virtual void New_Hand_IK(FVector InHand_R, FVector InHand_L);
	virtual void HandTraces_Movement(FVector InRightHandOffset, FVector InImpactPointRight, FVector InRightClavicleRot, FRotator InRightHandRotation, FVector InLeftHandOffset, FVector InImpactPointLeft, FVector InLeftClavicleRot, FRotator InLeftHandRotation);
	virtual void LegTraces(FVector InFoot_R, FRotator InFoot_r_Rot, FVector InFoot_R_Loc, FVector InFoot_L, FRotator InFoot_L_Rot, FVector Foot_L_Loc);
	virtual void Player_Offset(FVector InPelvis_Offset, FVector InHand_Offset, FVector InFoot_Offset);

	virtual void Moving(bool InIsMovingRight, bool InIsMovingLeft);
	virtual void TurnCorner(bool InIsTurnRight, bool InIsTurnLeft, bool InIsOuterCorner, bool InIsInnerCorner);
	virtual void Turn_Back(bool InIsTurnBack);

	virtual void AnimNotify_Hand_R();
	virtual void AnimNotify_Hand_L();
	

};

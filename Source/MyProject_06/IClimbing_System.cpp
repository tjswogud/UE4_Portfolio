#include "IClimbing_System.h"


void IClimbing_System::Enable_Climbing_System(bool InIsEnableClimbing)
{
}

void IClimbing_System::Input(float InMoveRight, float InMoveForward)
{
}

void IClimbing_System::States(bool InIsGrabLedge, bool InIsHanging)
{
}

void IClimbing_System::Reach(bool InCan_Reach)
{
}

void IClimbing_System::Jump_From(float InX, float InY, float InAngle)
{
}

void IClimbing_System::Freehang_Turn(bool InFreehang_Turn_Enable)
{
}

void IClimbing_System::Ledge_Climb_Up(bool IsLedge_Climb_Up)
{
}

void IClimbing_System::Climb_To_Ledge(float InAngle, bool InIsClimb, float InDistance, bool InIsShort_Climb,
	float InSide)
{
}

void IClimbing_System::Climb_Down_To_Ledge(bool InIsClimb_Down_Ledge)
{
}

void IClimbing_System::Enable_Freehang(bool InIsFreeHanging)
{
}

void IClimbing_System::Enable_Climb_Montage(bool InIsEnable_Climb_Montage_IK)
{
}

void IClimbing_System::Current_Hand_IK(FVector InHand_R, FVector InHand_L)
{
}

void IClimbing_System::New_Hand_IK(FVector InHand_R, FVector InHand_L)
{
}

void IClimbing_System::HandTraces_Movement(FVector InRightHandOffset, FVector InImpactPointRight,
	FVector InRightClavicleRot, FRotator InRightHandRotation, FVector InLeftHandOffset, FVector InImpactPointLeft,
	FVector InLeftClavicleRot, FRotator InLeftHandRotation)
{
}

void IClimbing_System::LegTraces(FVector InFoot_R, FRotator InFoot_r_Rot, FVector InFoot_R_Loc, FVector InFoot_L,
	FRotator InFoot_L_Rot, FVector Foot_L_Loc)
{
}

void IClimbing_System::Player_Offset(FVector InPelvis_Offset, FVector InHand_Offset, FVector InFoot_Offset)
{
}

void IClimbing_System::Moving(bool InIsMovingRight, bool InIsMovingLeft)
{
}

void IClimbing_System::TurnCorner(bool InIsTurnRight, bool InIsTurnLeft, bool InIsOuterCorner, bool InIsInnerCorner)
{
}

void IClimbing_System::Turn_Back(bool InIsTurnBack)
{
}

void IClimbing_System::AnimNotify_Hand_R()
{
}

void IClimbing_System::AnimNotify_Hand_L()
{
}

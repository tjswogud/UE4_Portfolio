#include "Component/CExecuteComponent.h"

#include "CMontagesComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/HumanType.h"
#include "Component/CWeaponComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Engine/DataTable.h"
#include "Character/CEnemy.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"

UCExecuteComponent::UCExecuteComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCExecuteComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AHumanType>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(AirExeHandle, this, &UCExecuteComponent::MoveToTarget, 0.01f, true, 0.0f);
	GetWorld()->GetTimerManager().PauseTimer(AirExeHandle);

	UCWeaponAsset** WeaponAssets = Owner->GetWeapons()->GetDataAssets();

	for(int i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if(WeaponAssets[i] != nullptr)
		{
			if(WeaponAssets[i]->ExecuteDataTable != nullptr)
			{
				TArray<FExecuteData*> datas;
				WeaponAssets[i]->ExecuteDataTable->GetAllRows<FExecuteData>("", datas);

				for (FExecuteData* data : datas)
				{
					ExecuteDatas.Push(*data);
				}
			}

			continue;
		}

		continue;
	}

	
}


void UCExecuteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(500, 100, 100));


	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	if (Owner->GetStatus()->GetPlaceType() == EPlaceType::InAir && (Owner->GetState()->IsIdleMode() || Owner->GetState()->IsJumpMode()))
	{
		UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), Owner->GetCapsuleComponent()->GetComponentLocation(), FVector(Owner->GetCapsuleComponent()->GetComponentLocation().X, Owner->GetCapsuleComponent()->GetComponentLocation().Y, Owner->GetCapsuleComponent()->GetComponentLocation().Z - 2000.0f), FVector(200, 200, 0.0f), FRotator::ZeroRotator, objects, false, ignores, EDrawDebugTrace::ForOneFrame, OutHitForAir, true);

	}
	
}

void UCExecuteComponent::PlayGroundExecution()
{

 	if (CheckCanGroundExecute())
	{

		Owner->SetActorLocation(HitResult.ImpactPoint);
		Owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Victim->GetActorLocation()));

		

		if(place == EExecutePlace::Behind)
		{
			Victim->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Victim->GetActorLocation(), Owner->GetActorLocation()) + FRotator(0,180,0));
		}
		else
		{
			Victim->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Victim->GetActorLocation(), Owner->GetActorLocation()));
		}
		


		FExecuteData Data;
		Data.WeaponType = Owner->GetWeapons()->GetCurWeaponType();
		Data.ExecutePlace = place;
		Data.IsSneak = Owner->GetState()->IsSneakMode();


		int indexnum = 0;

		for (int i = 0; i < ExecuteDatas.Num(); i++)
		{
			if (ExecuteDatas[i].WeaponType == Owner->GetWeapons()->GetCurWeaponType() && ExecuteDatas[i].ExecutePlace == place && ExecuteDatas[i].IsSneak == Data.IsSneak)
			{
				indexnum++;
			}
		}

		Data.index = FMath::RandRange(0, indexnum - 1);

		for (int i = 0; i < ExecuteDatas.Num(); i++)
		{
			if (ExecuteDatas[i].WeaponType == Owner->GetWeapons()->GetCurWeaponType() && ExecuteDatas[i].ExecutePlace == place && ExecuteDatas[i].index == Data.index && ExecuteDatas[i].IsSneak == Data.IsSneak)
			{
				Owner->GetState()->SetExecuteMode();
				Owner->PlayAnimMontage(ExecuteDatas[i].Montage, ExecuteDatas[i].PlayRate);
			}
		}



		Victim->GetMontages()->PlayExecuteMode(Data.WeaponType, Data.ExecutePlace, Data.index, Data.IsSneak);
	}

}

bool UCExecuteComponent::CheckCanGroundExecute()
{
	if (Owner->GetWeapons()->GetCurWeapon()->ExecuteDataTable == nullptr) return false;


	if (Owner != nullptr && !Owner->GetState()->IsExecuteMode() )
	{
		// Get the player's viewpoint
		FVector LineStart = Cast<ACPlayer>(Owner)->GetActorLocation();
		FVector LineEnd = (Owner->GetActorLocation() + Owner->GetActorForwardVector() * 1000);




		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner); // Ignore the player's own pawn

		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_Pawn,  Params);

		DrawDebugLine(Owner->GetWorld(), LineStart, LineEnd, FColor::Red, false, 1.0f, 0, 1.0f);


		//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green, false, 1.0f);

		if (HitResult.bBlockingHit)
		{


			if (Cast<ACEnemy>(HitResult.GetActor())) //적인지 아닌지
			{
				Victim = Cast<ACEnemy>(HitResult.GetActor());

				FVector temp = (Victim->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
				

				if(Cast<ACEnemy>(HitResult.GetActor())->GetState()->IsGroggyMode() && DecideExecutePlace(HitResult.Normal)) //그로기 상태인지 아닌지 && abs(FVector::DotProduct(temp, HitResult.ImpactNormal)	) < 0.3
				{
					return true;
				}


				if (!IsValid(Cast<ACEnemy>(HitResult.GetActor())->GetBlackboardComponent()->GetValueAsObject("Target")) && DecideExecutePlace(HitResult.Normal))
				{
					return true;
				}
			}

			return false;
		}


		return false;
	}

	return false;

	
}

void UCExecuteComponent::PlayAirExecution()
{
	Owner->GetCapsuleComponent()->SetWorldRotation(Victim->GetActorRotation());

	FExecuteData Data;
	Data.WeaponType = Owner->GetWeapons()->GetCurWeaponType();
	Data.ExecutePlace = EExecutePlace::Air;
	Data.IsSneak = Owner->GetState()->IsSneakMode();
	Data.index = UKismetMathLibrary::RandomInteger(4);

	AirVicData = Data;

	Owner->GetCapsuleComponent()->SetCollisionProfileName("Execute");


	for (int i = 0; i < ExecuteDatas.Num(); i++)
	{
		if (ExecuteDatas[i].WeaponType == Owner->GetWeapons()->GetCurWeaponType() && ExecuteDatas[i].ExecutePlace == Data.ExecutePlace && ExecuteDatas[i].index == Data.index && ExecuteDatas[i].IsSneak == Data.IsSneak)
		{
			Owner->GetState()->SetExecuteMode();

			AirAtkData = ExecuteDatas[i];

			Owner->PlayAnimMontage(ExecuteDatas[i].Montage, ExecuteDatas[i].PlayRate);
		}
	}



	Victim->GetMontages()->PlayExecuteMode(Data.WeaponType, Data.ExecutePlace, Data.index, Data.IsSneak);

	GetWorld()->GetTimerManager().UnPauseTimer(AirExeHandle);


}

bool UCExecuteComponent::CheckCanAirExecute()
{
	

	if (Owner->GetWeapons()->DataAssets[(uint8)Owner->GetWeapons()->GetCurWeaponType()]->ExecuteDataTable == nullptr) return false;
	if (Owner->GetStatus()->GetPlaceType() != EPlaceType::InAir) return false;
	if (!OutHitForAir.IsValidBlockingHit()) return false;

	if (Cast<ACEnemy>(OutHitForAir.GetActor()))
	{
		Victim = Cast<ACEnemy>(OutHitForAir.GetActor());

		float dot = FVector::DotProduct(Victim->GetActorForwardVector(), Owner->GetActorForwardVector());

		if ((Owner->GetActorLocation() - Victim->GetActorLocation()).Size() < 300)
		{
			OutHitForAir.Init();
			Victim = nullptr;
			return false;
		}

		if (dot > 0.7f && Victim->GetState()->IsIdleMode())
		{
			return true;
		}

	}

	return false;
}

bool UCExecuteComponent::DecideExecutePlace(FVector InImpactNormal)
{


	FVector VictimFront = Victim->GetActorForwardVector().GetSafeNormal();
	FVector VictimBehind = Victim->GetActorForwardVector().GetSafeNormal() * -1;
	FVector VictimUp = Victim->GetActorUpVector().GetSafeNormal();
	FVector VictimDown =Victim->GetActorUpVector().GetSafeNormal() * - 1 ;

	float maximum =-1.0f;

	float front = (FVector::DotProduct(VictimFront, InImpactNormal));
	float behind = (FVector::DotProduct(VictimBehind, InImpactNormal));
	float up = abs(FVector::DotProduct(VictimUp, InImpactNormal));
	float down = abs(FVector::DotProduct(VictimDown, InImpactNormal));

	maximum = FMath::Max(front, behind);


	if(maximum < 0.7)
	{
		return false;

		CLog::Print("Can't Execute");
	}
	else
	{
		//if (front == FMath::Min(FMath::Min3(front, behind, up), down))
		//	place = EExecutePlace::Front;
		//else if (behind == FMath::Min(FMath::Min3(front, behind, up), down))
		//	place = EExecutePlace::Behind;
		//else if (up == FMath::Min(FMath::Min3(front, behind, up), down))
		//	place = EExecutePlace::Top;
		//else if (down == FMath::Min(FMath::Min3(front, behind, up), down))
		//	place = EExecutePlace::Under;
		//else
		//	place = EExecutePlace::Max;

		if (front == FMath::Max(front, behind))
			place = EExecutePlace::Front;
		else if (behind == FMath::Max(front, behind))
			place = EExecutePlace::Behind;
		else 
			place = EExecutePlace::Max;

		CLog::Print(*UEnum::GetDisplayValueAsText(place).ToString());

		return true;
	}


}

void UCExecuteComponent::MoveToTarget()
{
	if (!IsValid(Victim)) return;



	FVector param1 = CHelpers::GetVector2D(Owner->GetActorLocation());
	FVector param2 = CHelpers::GetVector2D(Victim->GetActorLocation());

	FVector newlocation = UKismetMathLibrary::VLerp(param1, param2, 1.0f);
	


	//

	//FRotator OwnertoTarget = UKismetMathLibrary::FindLookAtRotation(Cast<ACPlayer>(Owner)->CameraLocation, Victim->GetActorLocation());

	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetActorRotation(OwnertoTarget);
	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetFOV(70.0f);


	Owner->SetActorLocation(FVector(newlocation.X, newlocation.Y, Owner->GetActorLocation().Z));

	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetFOV(70.0f);
;


	if(UKismetMathLibrary::EqualEqual_VectorVector(Owner->GetActorLocation(), Victim->GetActorLocation() + FVector(0, 0, 90.0f), 100.0f))
	{
		CLog::Print("AirExecute!");

		GetWorld()->GetTimerManager().PauseTimer(AirExeHandle);
		Owner->SetActorLocation(FVector(Victim->GetActorLocation().X, Victim->GetActorLocation().Y, Owner->GetActorLocation().Z));

		//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetFOV(90.0f);

		Owner->PlayAnimMontage(AirAtkData.Montage, AirAtkData.PlayRate, "Exec");

		Victim->GetMontages()->PlayExecuteMode(AirVicData.WeaponType, AirVicData.ExecutePlace, AirVicData.index, AirVicData.IsSneak, "Exec");




	}



}


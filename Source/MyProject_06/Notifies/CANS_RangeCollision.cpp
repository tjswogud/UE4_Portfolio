#include "Notifies/CANS_RangeCollision.h"
#include "Global.h"
#include "Component/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"

FString UCANS_RangeCollision::GetNotifyName_Implementation() const
{
	return "RangeCollision";
}

void UCANS_RangeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    Owner = Cast<AHumanType>(MeshComp->GetOwner());

    CheckNull(Owner);

    StartLocation = Owner->GetActorLocation() + Margin;
   

    DeltaTime = 0.0f;

    Hitted.Empty();

}

void UCANS_RangeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);


}

void UCANS_RangeCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

    FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), true, Owner);
    TraceParams.bTraceComplex = false;
    TraceParams.bReturnPhysicalMaterial = false;
    

	TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Radius);

    CheckNull(Owner);

    bool bHit = Owner->GetWorld()->SweepMultiByChannel(
        HitResults,
        StartLocation,
        StartLocation,
        FQuat::Identity,
        ECC_Pawn,
        CollisionShape,
        TraceParams
    );;

   


    

    DrawDebugSphere(
		Owner->GetWorld(),
        StartLocation,
        Radius,
        12,
        FColor::Red,
        false,
        5.0f,
        0,
        1.0f
    );


    if (bHit)
    {
        for (FHitResult result : HitResults)
        {

	        if(Cast<AHumanType>(result.GetActor()))
	        {

                Hitted.AddUnique(Cast<AHumanType>(result.GetActor()));
	        }
        }

    }

    if (Hitted.Num() == 0)
    {
    	return;

    }


    CheckNull(Owner);

    if (HitLoop)
    {


        if (DeltaTime == 0)
        {
            for (AHumanType* hitted : Hitted)
            {
                FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(hitted->GetActorLocation()), CHelpers::GetVector2D(Owner->GetActorLocation()));
                hitted->SetActorRotation(Rotator);
              
                HitData.SendDamage(Owner, Owner->GetWeapons()->GetCurWeapon()->GetAttachment(), hitted);
                
            }
        }
        else if (DeltaTime > HitTimer)
        {
            DeltaTime = 0;
        }



        DeltaTime += Owner->GetWorld()->DeltaTimeSeconds;
    }
    else
    {
        for (AHumanType* hitted : Hitted)
        {
            FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(hitted->GetActorLocation()), CHelpers::GetVector2D(Owner->GetActorLocation()));
            hitted->SetActorRotation(Rotator);
            HitData.SendDamage(Owner, Owner->GetWeapons()->GetCurWeapon()->GetAttachment(), hitted);

        }
    }




  

}

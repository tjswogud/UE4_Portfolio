#include "Component/CTargetComponent.h"
#include "Global.h"
#include "Containers/Map.h"
#include "Character/HumanType.h"
#include "Character/CEnemy.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    OwnerCharacter = Cast<AHumanType>(GetOwner());

}

void UCTargetComponent::ToggleTargeting()
{
	if(IsValid(Target))
	{
		End_Targeting();
	}
	else
	{
		Begin_Targeting();
	}

}

void UCTargetComponent::ChangeTargeting(bool IsRight)
{


    CheckNull(Target);

	TMap<float, AHumanType*> Map;
    TArray<float> arr;
    float Key = 0.0f;
    float Minimum = 2.0f;
    AHumanType* Candidate = nullptr;

    if(!MovingFocus)
    {

        MovingFocus = true;

        FVector StartLocation = OwnerCharacter->GetActorLocation();

        FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), true, OwnerCharacter);
        TraceParams.bTraceComplex = false;
        TraceParams.bReturnPhysicalMaterial = false;
        TraceParams.AddIgnoredActor(Target);

        TArray<FHitResult> HitResults;
        FCollisionShape CollisionShape;
        CollisionShape.SetSphere(Radius);


        CheckNull(OwnerCharacter);

        bool bHit = OwnerCharacter->GetWorld()->SweepMultiByChannel(
            HitResults,
            StartLocation,
            StartLocation,
            FQuat::Identity,
            ECC_Visibility,
            CollisionShape,
            TraceParams
        );;




        DrawDebugSphere(
            OwnerCharacter->GetWorld(),
            StartLocation,
            Radius,
            12,
            FColor::Red,
            false,
            5.0f,
            0,
            1.0f
        );



        if(bHit)
        {
            for (FHitResult Result : HitResults)
            {
                if(Cast<AHumanType>(Result.GetActor()))
                {
                    FVector temp = (Cast<AHumanType>(Result.GetActor())->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
                    FVector temp2 = FVector::CrossProduct(UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation()), temp);
					float key= FVector::DotProduct(temp2, OwnerCharacter->GetActorUpVector());

                    Map.Add(key, Cast<AHumanType>(Result.GetActor()));
                }
				
            }

            CheckTrue(Map.Num() <= 0);

            Map.GetKeys(arr);


            for (float key : arr)
            {
                Key = key;

                if(IsRight)
                {
                    if(Key > 0.0f)
                    {

                        if(abs(Key) < Minimum)
                        {
                            Minimum = abs(Key);

                            Candidate = *Map.Find(Key);
                        }


                    }

                }
                else
                {
                    if (Key < 0.0f)
                    {

                        if (abs(Key) < Minimum)
                        {
                            Minimum = abs(Key);

                            Candidate = *Map.Find(Key);
                        }


                    }
                }



            }

            CheckNull(Candidate);
            ChangeTarget(Candidate);

        }
    }


}

void UCTargetComponent::OnChangeTargeting(float InAxis)
{
    CheckNull(Target);

    if(InAxis > 0.0f)
    {
        ChangeTargeting(true);

    }
    else if(InAxis < 0.0f)
    {
        ChangeTargeting(false);

    }
}

void UCTargetComponent::Begin_Targeting()
{


    TArray<class AHumanType*> Candidates;

    FVector StartLocation = OwnerCharacter->GetActorLocation();

    FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), true, OwnerCharacter);
    TraceParams.bTraceComplex = false;
    TraceParams.bReturnPhysicalMaterial = false;


    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Radius);


    CheckNull(OwnerCharacter);

    bool bHit = OwnerCharacter->GetWorld()->SweepMultiByChannel(
        HitResults,
        StartLocation,
        StartLocation,
        FQuat::Identity,
        ECC_Pawn,
        CollisionShape,
        TraceParams
    );;




    DrawDebugSphere(
        OwnerCharacter->GetWorld(),
        StartLocation,
        Radius,
        12,
        FColor::Red,
        false,
        5.0f,
        0,
        1.0f
    );


    if(bHit)
    {
        for (FHitResult HitResult : HitResults)
        {
			if(Cast<AHumanType>(HitResult.GetActor()))
			{
                Candidates.AddUnique(Cast<AHumanType>(HitResult.GetActor()));
			}
        }

    }

    CheckTrue(Candidates.Num() <= 0);


    float dot = 0.5f;
    float distance = Radius;

    AHumanType* Candidate = nullptr;

    for (AHumanType* Human : Candidates)
    {
        FVector OwnertoTarget = Human->GetActorLocation() - OwnerCharacter->GetActorLocation();
        

        if(FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), OwnertoTarget) > dot && OwnertoTarget.Size() < distance)
        {
            distance = OwnertoTarget.Size();
            Candidate = Human;
        }


    }

    ChangeTarget(Candidate);

}

void UCTargetComponent::End_Targeting()
{
    if(Cast<ACEnemy>(Target))
    {
        Cast<ACEnemy>(Target)->TargetWidget->SetVisibility(false);
        Target = nullptr;
    }
}

void UCTargetComponent::ChangeTarget(AHumanType* Candidate)
{
    if (IsValid(Candidate))
    {
        if(IsValid(Target))
        {
            Cast<ACEnemy>(Target)->TargetWidget->SetVisibility(false);
        }
        
        Target = Candidate;
        Cast<ACEnemy>(Target)->TargetWidget->SetVisibility(true);

        if (Candidate->GetState()->IsDeadMode())
            End_Targeting();
    }
    else
    {
        End_Targeting();
    }



}

void UCTargetComponent::Tick_Targeting(float DeltaTime)
{
    FRotator ControlRotation = OwnerCharacter->GetControlRotation();

    FRotator OwnertoTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());

    

	if(ControlRotation.Equals(OwnertoTarget, FinishAngle))
	{
        OwnerCharacter->GetController()->SetControlRotation(OwnertoTarget);

        if(MovingFocus)
        {
            MovingFocus = false;
        }
	}
    else
    {
		FRotator temp =	UKismetMathLibrary::RInterpTo(ControlRotation, OwnertoTarget, DeltaTime, InterpSpeed);

        OwnerCharacter->GetController()->SetControlRotation(temp);
    }

}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


    if(Target == nullptr)
    {
        End_Targeting();
        return;
    }

    if(Target->GetState()->IsDeadMode())
    {
        End_Targeting();
    }

    CheckNull(Target);
    if(FVector::Dist(OwnerCharacter->GetActorLocation(), Target->GetActorLocation()) > Radius)
    {
        End_Targeting();
    }
    else
    {
        Tick_Targeting(DeltaTime);
    }

}


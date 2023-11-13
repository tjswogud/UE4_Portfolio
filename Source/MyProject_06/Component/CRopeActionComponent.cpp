#include "Component/CRopeActionComponent.h"
#include "CMontagesComponent.h"
#include "CMovementComponent.h"
#include "CStatusComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/CPlayer.h"
#include "Character/CEnemy_Boss.h"
#include "Character/HumanType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCRopeActionComponent::UCRopeActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Owner = Cast<AHumanType>(GetOwner());


	RopeClass = ACRope::StaticClass();

}


void UCRopeActionComponent::BeginPlay()
{
	Super::BeginPlay();


	if (!!RopeClass)
	{
		FActorSpawnParameters params;
		params.Owner = Owner;
		Rope = Owner->GetWorld()->SpawnActor<ACRope>(RopeClass, params);
	}

	if(Cast<ACPlayer>(Owner))
	{
		Rope->Owner = Cast<ACPlayer>(Owner);
	}
	else if(Cast<ACEnemy_Boss>(Owner))
	{
		Rope->Owner = Cast<ACEnemy_Boss>(Owner);
	}

	Rope->BeginPlay();
}


void UCRopeActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




	if(IsAimming)
	{
		IsActivate = true;

		if(Cast<ACPlayer>(Owner))
		{
			Tick_Aimming();
		}
	}
	else
	{
		if(IsActivate == false)
		{

		}
		else if(!HookPoint.IsNearlyZero())
		{
			if(Cast<ACEnemy_Boss>(Owner))
			{
				CLog::Print("Enemy beginHook");
			}
			BeginHookAction();
		}

	}


	if(Rope->IsEnter)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Owner->GetCharacterMovement()->GravityScale = 1.0f;

		float interSpeed = 4.0f	* (1.0f + (1.0f - (Owner->GetActorLocation() - HookPoint).Size() / 2500));
		float alpha = UKismetMathLibrary::FInterpTo(0, 1.0f, GetWorld()->GetDeltaSeconds(), interSpeed);


		FVector newLocation = UKismetMathLibrary::VLerp(Owner->GetActorLocation(), HookPoint, alpha);


		Owner->SetActorLocationAndRotation(newLocation, Owner->GetActorRotation(), false);



		if((Owner->GetActorLocation() - HookPoint).Size() <= FlyingEndDistance)
		{
			if(Cast<ACPlayer>(Owner))
			{
				Cast<ACPlayer>(Owner)->End_RopeAction();
			}
			else if(Cast<ACEnemy_Boss>(Owner))
			{
				Cast<ACEnemy_Boss>(Owner)->End_RopeAction();
			}

			Rope->SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Owner->GetMovement()->Move();
			Owner->GetMovement()->EnableControlRotation();
		}
	}
}

void UCRopeActionComponent::BeginAim()
{
	CheckTrue(Owner->GetState()->IsRopeActionMode());

	IsAimming = true;

	if(Cast<ACPlayer>(Owner))
	{
		Cast<ACPlayer>(Owner)->RopeAim->GetWidget()->AddToViewport();
	}
}

void UCRopeActionComponent::EndAim()
{
	if (Cast<ACPlayer>(Owner))
	{
		Cast<ACPlayer>(Owner)->RopeAim->GetWidget()->RemoveFromViewport();
		Cast<ACPlayer>(Owner)->RopeAim->GetWidget()->SetColorAndOpacity(FLinearColor::White);
	}

	IsAimming = false;
}

void UCRopeActionComponent::Tick_Aimming()
{
	


	FVector LineStart;
	FRotator Rotator;

	if(Cast<ACPlayer>(Owner))
	{
		LineStart = Cast<ACPlayer>(Owner)->CameraLocation;
		Rotator = Cast<ACPlayer>(Owner)->CameraRotator;
	}

	
	FVector LineEnd = LineStart + (UKismetMathLibrary::GetForwardVector(Rotator) * Distance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner); // Ignore the player's own pawn

	FHitResult HitResult;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_Visibility, Params);

	//DrawDebugLine(Owner->GetWorld(), LineStart, LineEnd, FColor::Green, false, 1.0f);

	if(HitResult.bBlockingHit)
	{
		if (Cast<ACPlayer>(Owner))
		{
			Cast<ACPlayer>(Owner)->RopeAim->GetWidget()->SetColorAndOpacity(FLinearColor::Red);
		}

		HookPoint = HitResult.ImpactPoint;
	}
	else
	{
		if (Cast<ACPlayer>(Owner))
		{
			Cast<ACPlayer>(Owner)->RopeAim->GetWidget()->SetColorAndOpacity(FLinearColor::White);
		}
		HookPoint = FVector::ZeroVector;
	}
}

void UCRopeActionComponent::BeginHookAction()
{
	//LaunchVector = (HookPoint - Owner->GetActorLocation()).GetSafeNormal() * LaunchVelocity;
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(Owner->GetActorLocation()), CHelpers::GetVector2D(HookPoint));
	Owner->SetActorRotation(Rotator);

	Owner->GetState()->SetRopeActionMode();
	Owner->GetMovement()->Stop();
	Owner->GetMovement()->DisableControlRotation();


	IsActivate = false;
}

void UCRopeActionComponent::ThrowHook()
{
	switch (Owner->GetStatus()->GetPlaceType()) {
	case EPlaceType::OnLand:


		break;
	case EPlaceType::InAir:
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Owner->GetCharacterMovement()->GravityScale = 0.0f;


		break;
	default:;
	}

	Rope->HookEndTrace = HookPoint;

	Rope->Active = true;

}

void UCRopeActionComponent::SettingHookPoint() //ai ¿ë
{
	if (HookPoints.Num() == 0) return;

	TArray<FVector> PossiblePoints;


	for (FVector point : HookPoints)
	{
		bool bCondition1 = (Owner->GetActorLocation() - point).Size() < 4000;
		bool bCondition2 = (Owner->GetActorLocation() - point).Size() > 500;

		if (bCondition1 && bCondition2)
		{
			PossiblePoints.Add(point);
		}
	}

	if (PossiblePoints.Num() == 0)
	{
		HookPoint = HookPoints[0];
		
	}
	else
	{
		HookPoint = PossiblePoints[UKismetMathLibrary::RandomInteger(PossiblePoints.Num())];

	}

	
	BeginHookAction();
}


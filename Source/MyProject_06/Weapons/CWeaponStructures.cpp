#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Character/HumanType.h"
#include "Component/CMontagesComponent.h"
#include "Sound/SoundWave.h"

void FDoActionData::DoAction(ACharacter * InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}


///////////////////////////////////////////////////////////////////////////////


void FSkillActionData::DoSkillAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

}

void FSkillActionData::DoComboAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (ComboAction.bFixedCamera)
			movement->EnableFixedCamera();

		if (ComboAction.bCanMove == false)
			movement->Stop();
	}

	if (!!ComboAction.Montage)
		InOwner->PlayAnimMontage(ComboAction.Montage, ComboAction.PlayRate);
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter * InAttacker, AActor * InAttackCauser, ACharacter * InOther, FVector InHitLocation)
{
	FActionDamageEvent e;
	e.HitData = this;
	e.HitData->HitLocation = InHitLocation;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter * InOwner)
{
	if (HittedType != EHittedType::MAX)
	{
		Cast<AHumanType>(InOwner)->GetMontages()->PlayHIttedAnim(HittedType, HitIndex);
	}
		
}

void FHitData::PlayHitStop(UWorld * InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));


	TArray<APawn*> pawns;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);

		if (!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f;

			pawns.Add(pawn);
		}
	}


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for (APawn* pawn : pawns)
			pawn->CustomTimeDilation = 1;
	});

	FTimerHandle handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundWave(ACharacter * InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Character/CEnemy_Range.h"
#include "Character/CPlayer.h"
#include "Weapons/CEquipment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/Attachments//CArrow.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponAsset.h"

UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionData, const FDoActionData& InDashActionData,
	const FDoActionData& InJumpActionData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InDashActionData, InJumpActionData);


	
	if(Cast<ACEnemy>(OwnerCharacter))
	{
		Bow_Attachment_Enemy = Cast<ACAttachment_Bow_Enemy>(InAttachment);
	}
	else
	{
		Bow_Attachment = Cast<ACAttachment_Bow>(InAttachment);
	}


	//SkeletalMesh = Bow_Attachment->SkeletalMesh;
	//PoseableMesh = Bow_Attachment->PoseableMesh;

	if (Cast<ACEnemy>(OwnerCharacter))
	{
		ACAttachment_Bow_Enemy* bow = Cast<ACAttachment_Bow_Enemy>(InAttachment);
		Bending = bow->GetBend();
	}
	else
	{

		ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
		Bending = bow->GetBend();
	}



	//OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();
}

void UCDoAction_Bow::DoAction()
{

	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}


	CheckFalse(State->IsIdleMode());


	if(State->IsSubActionMode())
	{
		Super::DoAction();
		DoActionDatas[3].DoAction(OwnerCharacter);
	}
	else
	{
		Super::DoAction();
		DoActionDatas[Index].DoAction(OwnerCharacter);
	}


}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);

}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();
	Index = 0;
	CreateArrow();

	

	if (Cast<ACEnemy>(OwnerCharacter))
	{
		Bow_Attachment_Enemy->StringHandAttach = true;;
	}
	else
	{
		Bow_Attachment->ShowTrajectory = false;
		Bow_Attachment->StringHandAttach = true;
	}
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();

}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);


	if(IsValid(GetAttachedArrow()))
	{
		GetAttachedArrow()->Destroy();
	}

	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}

	CheckNull(GetAttachedArrow());

	Arrows.Remove(GetAttachedArrow());
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	/*bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);*/

	if(*bEquipped == true && bBeginAction == false && bAttachedString == true)
	{
		
	}
	else
	{
		bCheck = false;
	}

	CheckFalse(bCheck);


}

void UCDoAction_Bow::Shoot(bool IsSkillShoot, FHitData* HitData, FVector InVelocity)
{
	if (InTrajectoryMode) return;

	bAttachedString = false;

	*Bending = 0;
	

	if (Cast<ACEnemy>(OwnerCharacter))
	{
		Bow_Attachment_Enemy->StringHandAttach = false;
	}
	else
	{
		Bow_Attachment->StringHandAttach = false;
	}




	CheckNull(ArrowClass);

	ACArrow* arrow = GetAttachedArrow();

	CheckNull(arrow);

	arrow->ActionIndex = Index;
	
	if(IsSkillShoot)
	{
		arrow->IsSkillShoot = IsSkillShoot;
		arrow->SkillHitData = *HitData;

		if(SkillIndex == 0)
		{
			Effect1 = UGameplayStatics::SpawnEmitterAttached(Cast<UParticleSystem>(SkillEffect1), arrow->StaticMeshComponent, "Socket", FVector::ZeroVector, FRotator(0, -90, 0), FVector::OneVector);
		}
	}

	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));



	arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
	arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

	FVector direction;

	if (Cast<ACPlayer>(OwnerCharacter))
	{
		FVector	LineStart = Cast<ACPlayer>(OwnerCharacter)->CameraLocation;
		FRotator Rotator = Cast<ACPlayer>(OwnerCharacter)->CameraRotator;
		LineEnd = LineStart + (UKismetMathLibrary::GetForwardVector(Rotator) * length);
		FRotator Rotator2 = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow"), LineEnd);

		direction = (LineEnd - OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow")).GetSafeNormal();

		if(!OwnerCharacter->GetWeapons()->GetSubAction()->GetInAction())
		{
			arrow->SetActorLocation(OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow") + UKismetMathLibrary::GetForwardVector(Rotator2) * 120.0f);

		}
		else
		{
			if(Cast<UCSubAction_Bow>(OwnerCharacter->GetWeapons()->GetSubAction())->IsblockingHit)
			{
				LineEnd = Cast<UCSubAction_Bow>(OwnerCharacter->GetWeapons()->GetSubAction())->LineEnd;
				Rotator2 = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow"), LineEnd);
				direction = (LineEnd - OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow")).GetSafeNormal();

			}
		}

		if(InVelocity.Equals(FVector::ZeroVector))
		{

			arrow->SetActorRotation(Rotator2);
			arrow->Shoot(direction);
		}
		else
		{
			arrow->Projectile->bRotationFollowsVelocity = true;

			if(Bow_Attachment->Direction < 0.0f)
			{
				arrow->Projectile->ProjectileGravityScale = 7.0f;
			}
			else if(Bow_Attachment->Direction > 0.0f && Bow_Attachment->Direction < 0.3f)
			{
				arrow->Projectile->ProjectileGravityScale = 5.0f;
			}
			else if (Bow_Attachment->Direction > 0.3f && Bow_Attachment->Direction < 0.6f)
			{
				arrow->Projectile->ProjectileGravityScale = 4.0f;
			}
			else if (Bow_Attachment->Direction > 0.6f)
			{
				arrow->Projectile->ProjectileGravityScale = 4.0f;
			}
			arrow->Shoot(InVelocity);
		}

	}
	else
	{
		FVector	LineStart = OwnerCharacter->GetActorLocation();
		LineEnd = LineStart + (UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotator(OwnerCharacter->GetControlRotation().Roll, Cast<ACEnemy_Range>(OwnerCharacter)->pitchRange, OwnerCharacter->GetControlRotation().Yaw))) * length;
		FRotator Rotator2 = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow"), LineEnd);
		direction = (LineEnd - LineStart).GetSafeNormal();

		arrow->SetActorLocation(OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow") + UKismetMathLibrary::GetForwardVector(Rotator2) * 120.0f);

		if (InVelocity.Equals(FVector::ZeroVector))
		{

			arrow->SetActorRotation(Rotator2);
			arrow->Shoot(direction);
		}
	}

	DrawDebugLine(OwnerCharacter->GetWorld(), OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow"), OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow") + direction * length, FColor::Red, false, 5.0f, 0.0f, 1.0f);

}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;
	bAttachedString = true;
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;


	FTransform transform;





	ACArrow* arrow;


	if (Cast<ACEnemy>(OwnerCharacter))
	{
		arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, Bow_Attachment_Enemy, OwnerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}
	else
	{
		arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, Bow_Attachment, OwnerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}


	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");



	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* projectile : Arrows)
	{
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}

	return nullptr;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	//CheckFalse(HitDatas.Num() > 0);

	//HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
	if(Cast<ACArrow>(InCauser)->IsSkillShoot)
	{

		if(SkillIndex == 2)
		{
			CHelpers::PlayEffect(GetWorld(), SkillEffect2, UKismetMathLibrary::MakeTransform(FVector::ZeroVector, FRotator::ZeroRotator, FVector(2.5,2.5,2.5)), Cast<ACArrow>(InCauser)->StaticMeshComponent, "Socket");
		}
		else if(SkillIndex == 0)
		{
			Effect1->DestroyComponent();
			CHelpers::PlayEffect(GetWorld(), SkillEffect2, UKismetMathLibrary::MakeTransform(FVector::ZeroVector, FRotator::ZeroRotator, FVector(2.5, 2.5, 2.5)), Cast<ACArrow>(InCauser)->StaticMeshComponent, "Socket");
		}

		Cast<ACArrow>(InCauser)->RangeAttack();
	}
	else
	{
		FHitData HitData;

		if (Cast<ACArrow>(InCauser)->ActionIndex == 2)
		{
			HitData.HittedType = EHittedType::StrongHit;
			HitData.PlayRate = 1.0f;
			HitData.Power = 10.0f;
			HitData.HitIndex = 0;
			if(IsValid(NormalEffect))
			{
				HitData.Effect = NormalEffect;
			}

		}
		else
		{
			HitData.HittedType = EHittedType::NormalHit;
			HitData.PlayRate = 1.0f;
			HitData.Power = 5.0f;
			HitData.HitIndex = 0;

			if (IsValid(NormalEffect))
			{
				HitData.Effect = NormalEffect;
			}
		}


		if (Cast<ACPlayer>(OwnerCharacter) && !InUseSkill)
		{
			if (HitData.HittedType == EHittedType::StrongHit)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes += 20;
			}
			else if (HitData.HittedType == EHittedType::NormalHit)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes += 10;
			}

			if (Cast<ACPlayer>(OwnerCharacter)->CurRes > Cast<ACPlayer>(OwnerCharacter)->MaxRes)
			{
				Cast<ACPlayer>(OwnerCharacter)->CurRes = Cast<ACPlayer>(OwnerCharacter)->MaxRes;
			}
		}



		HitData.SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

		
	}

	Arrows.Remove(Cast<ACArrow>(InCauser));
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

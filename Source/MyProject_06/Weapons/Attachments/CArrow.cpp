#include "Weapons/Attachments/CArrow.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "Character/HumanType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"

ACArrow::ACArrow()
{

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", RootComponent);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &StaticMeshComponent, "StaticMeshComponent", Capsule);




	Projectile->ProjectileGravityScale = 0;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);


	Ignores.Add(GetInstigator());
	Ignores.Add(GetOwner());

}

void ACArrow::BeginPlay_Implementation()
{
	Super::BeginPlay();



	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);


	Projectile->Deactivate();
}


void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	InDirection = InForward;
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	FTimerHandle TimerHandle;

	if(!IsValid(Cast<AHumanType>(GetInstigator())->GetWeapons()->GetCurWeapon()->GetSubAction()))
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		return;
	}

	if(!Cast<AHumanType>(GetInstigator())->GetWeapons()->GetCurWeapon()->GetSubAction()->GetInAction())
	{
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]() {
			// 딜레이 후 실행할 코드 작성
			Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}, 0.05f, false);

	}
	else
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	//Capsule->SetCollisionProfileName("BlockAll");
	//Capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}


void ACArrow::RangeAttack()
{

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetComponentLocation(), RangeRadius, objects, false, Ignores, EDrawDebugTrace::ForDuration, HitResults, true))
	{
		for (FHitResult result : HitResults)
		{
			if(Cast<AHumanType>(result.GetActor()))
			{
				Victims.AddUnique(Cast<AHumanType>(result.GetActor()));
			}
		}
	}

	if (IsValid(CameraShake))
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake);
	}


	for (AHumanType* vic : Victims)
	{
		SkillHitData.SendDamage(Cast<AHumanType>(GetInstigator()), GetOwner(), vic, Capsule->GetComponentLocation());
	}

}


void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
	{
		if(actor == OtherActor)
		{
			return;
		}
	}
		

	

	if(IsSkillShoot)
	{
		OnHit.Broadcast(this, nullptr);
	}
	else
	{
		ACharacter* character = Cast<ACharacter>(OtherActor);

		if (!IsValid(character)) return;

		if(Cast<ACPlayer>(character) && Cast<ACPlayer>(character)->IsBlocking)
		{
			Projectile->SetActive(false);
			Capsule->SetEnableGravity(true);
			Capsule->SetMassOverrideInKg(NAME_None, 0.1f, false);
			Capsule->SetSimulatePhysics(true);

			Capsule->AddRadialImpulse(Hit.ImpactPoint, 100, 100, ERadialImpulseFalloff::RIF_Linear, false);
			

			
		}
		else
		{
			Capsule->SetCollisionProfileName("NoCollision", false);
			Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FVector bonelocation = FVector::ZeroVector;
			FName temp = character->GetMesh()->FindClosestBone(Hit.ImpactPoint, &bonelocation);

			if (!bonelocation.Equals(FVector::ZeroVector))
			{
				FAttachmentTransformRules AttachrRules(
					EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true
				);


				this->Capsule->AttachToComponent(character->GetMesh(), AttachrRules, temp);
				//if()
				//{
				//	CLog::Print(temp.ToString());
				//}


			}
		}



		if (!!character && OnHit.IsBound())
			OnHit.Broadcast(this, character);
	}



}
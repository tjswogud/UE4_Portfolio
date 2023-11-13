#include "CRope.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
#include "Character/HumanType.h"
#include "Component/CMontagesComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACRope::ACRope() 
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &ProjectileMovement, "ProjectileMovement");
	CHelpers::CreateActorComponent(this, &CableComponent, "CableComponent");

	CHelpers::CreateComponent(this, &SphereComponent, "SphereComponent");
	CHelpers::CreateComponent(this, &StaticMeshComponent, "Hook", SphereComponent);
		
	

	ProjectileMovement->InitialSpeed = ThrowSpeed;
	ProjectileMovement->MaxSpeed = ThrowSpeed;


	CableComponent->CableWidth = RopeThickness;
	CableComponent->CableGravityScale = 0.0f;

	if(!!RopeMaterial)
		CableComponent->SetMaterial(0, RopeMaterial);



	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACRope::OnSphereComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACRope::OnSphereComponentEndOverlap);
	}


	

}

void ACRope::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(Owner);


	CableComponent->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RopeSocket");


	CableComponent->SetAttachEndTo(this, NAME_None);
	HookSocketLocation = Owner->GetMesh()->GetSocketLocation("RopeSocket");


	CableComponent->SetVisibility(false);
	CableComponent->SetAttachEndToComponent(SphereComponent);

	StaticMeshComponent->SetStaticMesh(HookMesh);
	StaticMeshComponent->SetVisibility(false);
}

void ACRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpToPoint();

}

void ACRope::InterpToPoint()
{
	

	if(Active)
	{
		CheckTrue(IsEnter)
		StaticMeshComponent->SetVisibility(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		
		CableComponent->SetVisibility(true);
		float interSpeed = ThrowSpeed * (1.0f + (1.0f - (HookSocketLocation - HookEndTrace).Size() / ThrowDistance));
		float alpha = UKismetMathLibrary::FInterpTo(0, 1.0f, GetWorld()->GetDeltaSeconds(), interSpeed);


		FVector newLocation = UKismetMathLibrary::VLerp(GetActorLocation(), HookEndTrace + MovingActorOffset, alpha);


		SetActorLocationAndRotation(newLocation, GetActorRotation(), false, HitResult);


		StaticMeshComponent->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HookEndTrace - HookSocketLocation));

		//CableComponent->CableLength = (HookEndTrace - Owner->GetMesh()->GetSocketLocation("RopeSocket")).Size2D();


	}
	else
	{
		IsEnter = false;
		CableComponent->SetVisibility(false);
		StaticMeshComponent->SetVisibility(false);
		this->SetActorLocation(Owner->GetMesh()->GetSocketLocation("RopeSocket"));
	}


}

void ACRope::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Owner) return;



	switch (Owner->GetStatus()->GetPlaceType()) {
	case EPlaceType::OnGround: break;
	case EPlaceType::OnLand:
		Owner->GetMontages()->PlayAnimMontage(EStateType::RopeAction, 0, "Enter");
		break;
	case EPlaceType::OnWall: break;
	case EPlaceType::InAir:
		Owner->GetMontages()->PlayAnimMontage(EStateType::RopeAction, 1, "Enter");
		break;
	default:;
	}

	if (IsValid(RopePullSound))
	{
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, RopePullSound); // 사운드 재생을 위한 UAudioComponent 생성
		if (AudioComponent)
		{
			AudioComponent->Play(); // 사운드 재생
		}
	}

	if(IsEnter == false)
	{
		if(Cast<ACEnemy>(OtherActor))
		{
			HitData.SendDamage(Owner, this, Cast<ACEnemy>(OtherActor));
		}

		

		float DelayTime = 0.3f; // 딜레이 시간 (초 단위)
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]() {
			IsEnter = true;
		}, DelayTime, false);

	}
}

void ACRope::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


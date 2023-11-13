#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/HumanType.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Root, "Root");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<AHumanType>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

			Collisions.Add(shape);
		}
	}
	OffCollisions();

	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::AttachToCollision(FName InSocketName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InSocketName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

			return;
		}
	}
}

void ACAttachment::OnCollisions()
{
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACAttachment::OffCollisions()
{
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));

}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));

}

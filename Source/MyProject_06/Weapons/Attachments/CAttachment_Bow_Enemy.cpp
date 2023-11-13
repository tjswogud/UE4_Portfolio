#include "Weapons/Attachments/CAttachment_Bow_Enemy.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Components/PoseableMeshComponent.h"
#include "Weapons/AnimInstance/CAnimInstance_Bow.h"

float* ACAttachment_Bow_Enemy::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}

ACAttachment_Bow_Enemy::ACAttachment_Bow_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);
}

void ACAttachment_Bow_Enemy::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);


	OriginLocation = SkeletalMesh->GetBoneLocation("bow_string_mid", EBoneSpaces::ComponentSpace);


}

void ACAttachment_Bow_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	if (StringHandAttach)
	{
		FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
		PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
	}
	else
	{
		PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);
	}

}

void ACAttachment_Bow_Enemy::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();


	AttachTo("Hand_Bow_Left");

	StringHandAttach = true;
}

void ACAttachment_Bow_Enemy::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");

	StringHandAttach = false;

	
}

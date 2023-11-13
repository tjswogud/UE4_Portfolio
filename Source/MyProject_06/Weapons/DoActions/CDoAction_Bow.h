#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/Attachments/CAttachment_Bow_Enemy.h"
#include "CDoAction_Bow.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_06_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
		UFXSystemAsset* NormalEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
		UFXSystemAsset* SkillEffect1;


	UPROPERTY(EditAnywhere, Category = "Effect")
		UFXSystemAsset* SkillEffect2;
public:
	UCDoAction_Bow();

	virtual void BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionData, const FDoActionData& InDashActionData, const FDoActionData& InJumpActionData) override;

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	

	virtual void OnBeginEquip() override;
	virtual void OnUnequip() override;

	virtual void Tick(float InDeltaTime) override;

	void Shoot(bool IsSkillShoot, FHitData* HitData = nullptr, FVector InVelocity = FVector::ZeroVector);

public:
	void End_BowString();

public:
	void CreateArrow();
	class ACArrow* GetAttachedArrow();

public:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter = nullptr);

	UFUNCTION()
		void OnArrowEndPlay(class ACArrow* InDestroyer);

	UPROPERTY(EditAnywhere)
		float length = 1500.0f;

public:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

public:
	float* Bending;

public:
	FVector OriginLocation;
	bool bAttachedString = true;

public:
	const bool* bEquipped;
	bool InTrajectoryMode = false;

public:
	TArray<class ACArrow*> Arrows;
	ACAttachment_Bow* Bow_Attachment;
	ACAttachment_Bow_Enemy* Bow_Attachment_Enemy;
	
public:
	FVector LineEnd;

	UPROPERTY()
		UParticleSystemComponent* Effect1;

	UPROPERTY()
		UParticleSystemComponent* Effect2;
};

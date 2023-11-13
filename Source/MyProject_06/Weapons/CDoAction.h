#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/HumanType.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_06_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	bool& GetExist() { return bExist; }
	int32& GetComboIndex() { return Index; }

public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const FDoActionData& InDashActionData, 
		const FDoActionData& InJumpActionData
	);

	virtual void Tick(float InDeltaTime) { }

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	virtual void Do_SubAction();
	virtual void Begin_SubAction();
	virtual void End_SubAction();


	virtual void Do_DashAction();
	virtual void Do_JumpAction();

	bool& GetBeginAction() { return bBeginAction; }

public:
	UFUNCTION()
		virtual void OnEquip() { }

	UFUNCTION()
		virtual void OnBeginEquip() { }

	UFUNCTION()
		virtual void OnEndEquip() { }

	UFUNCTION()
		virtual void OnUnequip() { }

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

	TArray<FDoActionData> GetDoActionDatas() { return DoActionDatas; }
	FDoActionData GetDashActionData() { return DashActionData; }
	FDoActionData GetJumpActionData() { return JumpActionData; }

public:
	bool InUseSkill = false;
	bool InUseSkillCombo = false;

	int32 SkillIndex = 0;
protected:
	bool bBeginAction;

	AHumanType* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	FDoActionData DashActionData;
	FDoActionData JumpActionData;
	//TArray<FHitData> HitDatas;


protected:
	FHitData LocalHitData;


protected:
	int32 Index = 0;

	bool bEnable;
	bool bExist;


};

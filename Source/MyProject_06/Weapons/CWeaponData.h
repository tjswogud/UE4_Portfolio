#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class MYPROJECT_06_API UCWeaponData : public UObject
{
	GENERATED_BODY()

public:
	friend class UCWeaponAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }

public:
	TArray<FSkillActionData> SkillActionDatas;
	TArray<FDoActionData> DeflectActionDatas;


	UDataTable* ExecuteDataTable;

public:

	TArray<FDoActionData> GetDeflectActionDatas() { return DeflectActionDatas; }
	TArray<FSkillActionData> GetSkillActionDatas() { return SkillActionDatas; }



private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;

	UPROPERTY()
		class UCSubAction* SubAction;
};

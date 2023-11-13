#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class MYPROJECT_06_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		FDoActionData DashActionData;

	UPROPERTY(EditAnywhere)
		FDoActionData JumpActionData;


	UPROPERTY(EditAnywhere)
		TArray<FSkillActionData> SkillActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DeflectActionDatas;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

public:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* ExecuteDataTable;

	

public:
	UCWeaponAsset();


	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);
	void CopyData(const UCWeaponAsset& Other, class ACharacter* Owner);

//public:
//	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
//	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
//	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
//	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }
//
//	 

//public:
//	
//
//	TArray<FDoActionData> GetDeflectActionDatas() { return DeflectActionDatas; }
//	TArray<FSkillActionData> GetSkillActionDatas() { return SkillActionDatas; }
//
//private:
//	UPROPERTY()
//		class ACAttachment* Attachment;
//
//	UPROPERTY()
//		class UCEquipment* Equipment;
//
//	UPROPERTY()
//		class UCDoAction* DoAction;
//
//	UPROPERTY()
//		class UCSubAction* SubAction;

private:

};

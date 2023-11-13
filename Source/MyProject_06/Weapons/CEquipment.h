#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class MYPROJECT_06_API UCEquipment : public UObject
{
	GENERATED_BODY()
public:
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	virtual void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	virtual void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	virtual void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	virtual void Unequip_Implementation();

public:
	FORCEINLINE bool IsEquipped() { return bEquipped; }

public:
	FEquipmentBeginEquip OnEquipmentEquip;
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentBeginEquip OnEquipmentEndEquip;
	FEquipmentUnequip OnEquipmentUnequip;
private:
	class AHumanType* OwnerCharacter;
	FEquipmentData Data;

private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;
};

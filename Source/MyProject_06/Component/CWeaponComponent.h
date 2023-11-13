#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"

#include "CWeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist = 0, Sword, Hammer, Warp, Around, Bow, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
		EWeaponType Type = EWeaponType::Max;

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode() { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode() { return Type == EWeaponType::Hammer; }
	FORCEINLINE bool IsWarpMode() { return Type == EWeaponType::Warp; }
	FORCEINLINE bool IsAroundMode() { return Type == EWeaponType::Around; }
	FORCEINLINE bool IsBowMode() { return Type == EWeaponType::Bow; }


	FORCEINLINE UCWeaponAsset** GetDataAssets() { return DataAssets; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EWeaponType GetCurWeaponType() { return Type; }

	UFUNCTION(BlueprintPure)
		class UCWeaponData* GetCurWeapon() { return Datas[(int32)Type]; }

	UFUNCTION(BlueprintCallable)
		void HiddenAttachment(bool IsHidden);

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool IsIdleMode();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();

public:
	void SetUnarmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetWarpMode();
	void SetAroundMode();
	void SetBowMode();

	void DoAction();
	void DashAction();
	void JumpAction();

	void SubAction_Pressed();
	void SubAction_Released();

	void SkillAction(int Inindex);


private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);


public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class AHumanType* Owner;
	bool IsUsingSkill;




public:
	TMap<EWeaponType, UCWeaponAsset*> WeaponDataAssets;

public:
	UPROPERTY()
		UCWeaponData* Datas[(int32)EWeaponType::Max];
};

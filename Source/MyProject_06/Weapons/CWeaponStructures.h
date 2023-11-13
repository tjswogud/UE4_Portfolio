#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Component/CWeaponComponent.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"


UENUM()
enum class EHittedType : uint8
{
	NormalHit, StrongHit, GroundHit, UpperHit, MAX
};

UENUM()
enum class EExecutePlace : uint8
{
	Front, Behind, Air, Ledge, Max
};


USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};


USTRUCT()
struct FExecuteData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
		EExecutePlace ExecutePlace;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		int index = 0;

	UPROPERTY(EditAnywhere)
		bool IsSneak = false;
};


USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void DoAction(class ACharacter* InOwner);
};


USTRUCT()
struct FSkillActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "ComboAction")
		FDoActionData ComboAction;

public:
	bool bCanCombo;

public:
	void DoSkillAction(class ACharacter* InOwner);
	void DoComboAction(class ACharacter* InOwner);

};


USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EHittedType HittedType = EHittedType::MAX;

	UPROPERTY(EditAnywhere)
		int HitIndex = -1;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float Launch = 100;

	UPROPERTY(EditAnywhere)
		float StopTime = 0.0f;

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound= nullptr;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect = nullptr;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
		FVector HitLocation = FVector::ZeroVector;
public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FVector InHitLocation = FVector::ZeroVector);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FActionDamageEvent
	: public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
};

UCLASS()
class MYPROJECT_06_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};

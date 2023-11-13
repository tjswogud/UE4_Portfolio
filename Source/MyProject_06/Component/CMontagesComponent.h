#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Weapons/CWeaponStructures.h"
#include "Component/CStateComponent.h"
#include "CMontagesComponent.generated.h"


USTRUCT()
struct FMontagesData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		int Index = 0;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;
};

USTRUCT()
struct FHitMontagesData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EHittedType HittedType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		int Index = -1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;

	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* LandHitDataTable;

	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* AirHitDataTable;

	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* ExecuteDataTable;

public:
	UPROPERTY()
		TArray<FHitMontagesData> LandHittedData;

	UPROPERTY()
		TArray<FHitMontagesData> AirHittedData;

	UPROPERTY()
		TArray<FExecuteData> ExecuteDatas;

	UPROPERTY()
		TArray<FMontagesData> Datas;

public:

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayDodgeMode();
	void PlayDeadMode();
	void PlayGroundMode();
	void PlayRiseMode();
	void PlayExecuteMode(EWeaponType InWeaponType, EExecutePlace InExecutePlace, int index, bool IsSneak, FName Name = NAME_None);


	void PlayHIttedAnim(EHittedType InType, int Index = -1);
	void PlayAnimMontage(EStateType InType, int Index = 0);
	void PlayAnimMontage(EStateType InType, int Index, FName StartSelectionName);

	void PlayRandomIndexOfAnim(EStateType InType);

	
private:
	class AHumanType* Owner;

};

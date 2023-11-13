#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Component/CWeaponComponent.h"
#include "CExecuteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCExecuteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ExecuteData")
		TArray<FExecuteData> ExecuteDatas;



public:	
	UCExecuteComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayGroundExecution();
	bool CheckCanGroundExecute();

	void PlayAirExecution();
	bool CheckCanAirExecute();

private:

	bool DecideExecutePlace(FVector InImpactNormal);

	void MoveToTarget();

public:
	class ACEnemy* GetVictim() { return Victim; }
	
private:
	class AHumanType* Owner;
	ACEnemy* Victim;

	EExecutePlace place;
	FHitResult HitResult;

	FHitResult OutHitForAir;

	FTimerHandle AirExeHandle;

	FExecuteData AirAtkData;
	FExecuteData AirVicData;
};

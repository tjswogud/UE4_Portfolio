#pragma once

#include "CoreMinimal.h"
#include "CRope.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "CRopeActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCRopeActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		UWidgetComponent* AimWidget;


	UPROPERTY(EditAnywhere)
		TSubclassOf<ACRope> RopeClass;


	UPROPERTY(EditAnywhere)
		float LaunchVelocity = 1500.0f;

	UPROPERTY(EditAnywhere)
		TArray<FVector> HookPoints;

	UPROPERTY(EditAnywhere)
		float FlyingEndDistance = 100;

	UPROPERTY(EditAnywhere)
		float Distance = 4000.0f;

public:	
	UCRopeActionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginAim();
	void EndAim();
	void Tick_Aimming();

	void BeginHookAction();

	void ThrowHook();

	void SettingHookPoint();

	ACRope* GetRope() { return Rope; }
private:


private:
	class AHumanType* Owner;

	FVector* StartPosition;
	FRotator* StartRotation;

	FVector HookPoint = FVector::ZeroVector;


	

	bool IsAimming = false;



public:

	bool IsActivate = false;
public:
	FVector LaunchVector;

	class ACRope* Rope;
};

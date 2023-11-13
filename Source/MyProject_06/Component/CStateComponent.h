#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, Dodge, Equip, Hitted, Dead, Action, Ground, Rise, Sneak, Hide, Groggy, Execute, Parried, Jump, RopeAction, Climb ,Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		bool IsFront = true;

	UPROPERTY(BlueprintReadWrite)
		bool IsGround = false;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE	bool IsIdleMode() { return Type == EStateType::Idle; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDodgeMode() { return Type == EStateType::Dodge; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGroundMode() { return Type == EStateType::Ground; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRiseMode() { return Type == EStateType::Rise; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSneakMode() { return Type == EStateType::Sneak; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHideMode() { return Type == EStateType::Hide; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGroggyMode() { return Type == EStateType::Groggy; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsExecuteMode() { return Type == EStateType::Execute; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsParriedMode() { return Type == EStateType::Parried; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsJumpMode() { return Type == EStateType::Jump; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRopeActionMode() { return Type == EStateType::RopeAction; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsClimbMode() { return Type == EStateType::Climb; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE FString GetCurType() { return *UEnum::GetDisplayValueAsText(Type).ToString(); }


	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

public:
	UCStateComponent();

public:
	void SetIdleMode();
	void SetDodgeMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetGroundMode();
	void SetRiseMode();
	void SetSneakMode();
	void SetHideMode();
	void SetGroggyMode();
	void SetExecuteMode();
	void SetParriedMode();
	void SetJumpMode();
	void SetRopeActionMode();
	void SetClimbMode();


	void OnSubActionMode();
	void OffSubActionMode();

	EStateType GetPrevType() { return PrevType; }
private:
	void ChangeType(EStateType InType);

protected:
	virtual void BeginPlay() override;

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
	EStateType PrevType;

	bool bInSubActionMode;


public:
	int MaxJumpCount = 2;
	int CurJumpCount = 0;
};

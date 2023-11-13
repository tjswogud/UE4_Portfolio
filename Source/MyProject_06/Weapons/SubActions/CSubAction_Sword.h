#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Sword.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_06_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()

public:
	virtual void Pressed() override;
	virtual void Released() override;
	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;

	
private:
	UFUNCTION()
	void StartParry();
	UFUNCTION()
	void EndParry();

	FTimerHandle End_ParryTimer;
};

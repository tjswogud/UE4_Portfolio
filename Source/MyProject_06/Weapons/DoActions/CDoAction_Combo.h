#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class MYPROJECT_06_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
	


public:
	UFUNCTION()
		void GetHitData(FHitData InData);

public:
	TArray<class ACharacter*> GetHittedActors() { return Hitted; }

public:
	virtual void DoAction() override;
	virtual	void Begin_DoAction() override;
	virtual	void End_DoAction() override;

	virtual void Do_DashAction() override;
	virtual void Do_JumpAction() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	virtual void OnAttachmentEndCollision() override;

	virtual void Tick(float InDeltaTime) override;

	

private:
	TArray<class ACharacter*> Hitted;

};

#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Melee.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_06_API ACEnemy_Melee : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_Melee();

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

private:

	virtual void Tick(float DeltaSeconds) override;

	virtual void Hitted() override;

public:
	virtual void End_Hitted() override;

private:
	virtual void Dead() override;

public:
	virtual void End_Dead() override;


public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:
	virtual void AI_Attack() override;
	virtual void AI_Avoid() override;
	virtual void AI_Block() override;
	virtual void AI_Skill() override;

	
private:
	virtual void Equip() override;


};

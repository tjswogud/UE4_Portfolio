#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Boss.generated.h"

UCLASS()
class MYPROJECT_06_API ACEnemy_Boss : public ACEnemy
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCRopeActionComponent* RopeActionComponent;

public:
	ACEnemy_Boss();

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


	virtual void Landed(const FHitResult& Hit) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void AI_Attack() override;
	virtual void AI_Avoid() override;
	virtual void AI_Block() override;
	virtual void AI_Skill() override;

	virtual void AI_RopeAction() override;
	

	void End_RopeAction();

private:
	virtual void Equip() override;
};




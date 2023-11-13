#pragma once

#include "CoreMinimal.h"
#include "Character/HumanType.h"
#include "Components/WidgetComponent.h"
#include "CPlayer.generated.h"



UCLASS()
class MYPROJECT_06_API ACPlayer : public AHumanType
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCExecuteComponent* Execute;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCTargetComponent* Targeting;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCRopeActionComponent* RopeActionComponent;

	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UCFeetComponent* FeetComponent;
public:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Climbing_Sphere;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Climbing_Sphere2;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UWidgetComponent* RopeAim;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UWidgetComponent* GroggyBar;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* SwordSkillWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* BowSkillWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		UWidgetComponent* BossClearWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Resource")
		float CurRes = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Resource")
		float MaxRes = 300.0f;


public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaSeconds) override;

public:

	void Sneak();
	void Hide();
	void MainAction();
	

	void DodgeAction();

private:
	void GetMouseWheelAxis(float InAxisValue);


public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:
	virtual void Hitted() override;
	virtual void Dodge() override;
	void RopeAction();


public:
	virtual void End_Hitted() override;
	virtual void End_Dodge() override;
	void End_RopeAction();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

private:


	void Skill1();
	void Skill2();
	void Skill3();
	void Skill4();

private:
	bool IsSprintMode;

	int skillIndex = 0;

public:
	FRotator CameraRotator;
	FRotator CapsuleRotator;

	FVector CameraLocation;

public:
	float MouseWheelAxis = 0.0f;

};

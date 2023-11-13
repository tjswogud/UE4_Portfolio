#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Radius = 3000.0f;

	UPROPERTY(EditAnywhere)
		float FinishAngle = 5.0f;

	UPROPERTY(EditAnywhere)
		float InterpSpeed = 15.0f;

public:	
	UCTargetComponent();

public:
	void ToggleTargeting();
	void ChangeTargeting(bool IsRight);

	void OnChangeTargeting(float InAxis);

private:
	void Begin_Targeting();
	void End_Targeting();
	void ChangeTarget(class AHumanType* Candidate);
	void Tick_Targeting(float DeltaTime);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	class AHumanType* OwnerCharacter;
	class AHumanType* Target;



	bool MovingFocus = false;
};

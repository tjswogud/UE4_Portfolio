#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"


UCLASS()
class MYPROJECT_06_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed();
	virtual void Released();




public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	class AHumanType* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;


};

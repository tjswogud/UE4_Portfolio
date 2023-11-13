#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
		FVector CameraLocation;
};



UCLASS(Blueprintable)
class MYPROJECT_06_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float AimingSpeed = 200;




public:
	UCSubAction_Bow();

public:
	virtual void Pressed() override;
	virtual void Released() override;

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
	virtual void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
		void OnAiming(FVector Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FTimeline Timeline;

private:
	FAimData OriginData;

private:
	float* Bend;


public:
	ACAttachment_Bow* Bow_Attachment;
	FVector LineEnd;
	bool IsblockingHit;

};

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Weapons/CAttachment.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "CAttachment_Bow.generated.h"


UCLASS()
class MYPROJECT_06_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40, +30);

public:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* PoseableMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Grid_Sphere;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UWidgetComponent* CrossHairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UUserWidget* UserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USplineComponent* Spline_Path;

	UPROPERTY(EditAnywhere)
		UStaticMesh* SplineMesh;



public:
	float* GetBend();

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ArrowTrajectory();

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

	void MouseWheelAxisValue(float AxisValue);

private:
	FVector2D OriginViewPitchRange;

	FVector OriginLocation;

public:
	bool StringHandAttach = false;

public:
	FHitResult HitResult;
	TArray<FVector> vectorPos;
	TArray<USplineMeshComponent*> points_Array;
	FVector LastTracePos;

	FVector ArrowVelocity = FVector::ZeroVector;
public:
	float Direction = 0.0f;
	float Speed = 1000.0f; 


	bool ShowTrajectory = false;
};

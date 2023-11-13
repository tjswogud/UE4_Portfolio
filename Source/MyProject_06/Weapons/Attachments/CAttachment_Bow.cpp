#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Character/HumanType.h"
#include "Character/CPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/AnimInstance/CAnimInstance_Bow.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<USplineComponent>(this, &Spline_Path, "Spline_Path", SkeletalMesh, "arrow");
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);
	CHelpers::CreateComponent<UWidgetComponent>(this, &CrossHairWidget, "CrossHairWidget", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Grid_Sphere, "Grid_Sphere", Root);


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);


	
	CrossHairWidget->SetVisibility(false);

	
	Grid_Sphere->SetVisibility(false);
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);


	OriginLocation = SkeletalMesh->GetBoneLocation("bow_string_mid", EBoneSpaces::ComponentSpace);


	UserWidget = CrossHairWidget->GetUserWidgetObject();
	UserWidget->AddToViewport();
	UserWidget->SetVisibility(ESlateVisibility::Hidden);


}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	if(StringHandAttach)
	{
		FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
		PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
	}
	else
	{
		PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);
	}


	if(OwnerCharacter->GetWeapons()->IsBowMode())
	{
		if(ShowTrajectory)
		{
			ArrowTrajectory();
		}
	}
	
}

void ACAttachment_Bow::ArrowTrajectory()
{
	CheckNull(OwnerCharacter->GetWeapons());
	CheckNull(OwnerCharacter->GetWeapons()->GetDoAction());
	if (!OwnerCharacter->GetWeapons()->GetDoAction()->InUseSkill)
	{

		for (USplineMeshComponent* point : points_Array)
		{
			point->DestroyComponent();
		}

		points_Array.Empty();

		Spline_Path->ClearSplinePoints(true);
		Grid_Sphere->SetVisibility(false);
		return;
	}

	
	Grid_Sphere->SetVisibility(true);

	TArray<AActor*> ignores;
	ignores.Add(GetInstigator());


	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FVector Start = SkeletalMesh->GetSocketTransform("Start",RTS_World).GetLocation();
	FVector velocity = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotator(SkeletalMesh->GetSocketTransform("Start", RTS_World).Rotator().Roll, SkeletalMesh->GetSocketTransform("arrow", RTS_World).Rotator().Pitch, OwnerCharacter->GetControlRotation().Yaw));

	ArrowVelocity = velocity;



	float lerpvalue = UKismetMathLibrary::Lerp(Direction, Cast<ACPlayer>(OwnerCharacter)->MouseWheelAxis / 50.0f + Direction, 10.0f);

	for(USplineMeshComponent* point : points_Array)
	{
		point->DestroyComponent();
	}

	points_Array.Empty();

	Spline_Path->ClearSplinePoints(true);

	if(lerpvalue < 0.7 && lerpvalue > -0.3)
	{ 
		Direction = lerpvalue;
	}

	velocity = Direction* velocity + velocity;

	UGameplayStatics::PredictProjectilePath(GetWorld(), HitResult, vectorPos, LastTracePos, Start, velocity * Speed, true, 20.0f, objects, false, ignores, EDrawDebugTrace::None, 0.0f, 20);


	for (int32 i = 0; i < vectorPos.Num(); i++)
	{

		Spline_Path->AddSplinePointAtIndex(vectorPos[i], i, ESplineCoordinateSpace::World, true);
	}


	

	Spline_Path->SetSplinePointType(vectorPos.Num() - 1, ESplinePointType::CurveClamped, true);

	for (int32 i = 0; i < Spline_Path->GetNumberOfSplinePoints() - 2; i++)
	{
		FTransform l_Transform;// = UKismetMathLibrary::MakeTransform(FVector::ZeroVector, FRotator(0, 0, 90.0f), FVector::OneVector);


		USplineMeshComponent* tempPoint = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, l_Transform, false));
		tempPoint->SetMobility(EComponentMobility::Movable);
		tempPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		tempPoint->SetForwardAxis(ESplineMeshAxis::Z, true);
		tempPoint->SetMaterial(0, Material);
		tempPoint->SetStartScale(FVector2D(0.1f, 0.1f), true);
		tempPoint->SetEndScale(FVector2D(0.1f, 0.1f), true);

		if(IsValid(SplineMesh))
		{
			tempPoint->SetStaticMesh(SplineMesh);

		}
			

		points_Array.Add(tempPoint);

		tempPoint
		->SetStartAndEnd(
			vectorPos[i],
			Spline_Path->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World),
			vectorPos[i + 1],
			Spline_Path->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World),
			true
			);


	}



	if (!IsValid(Grid_Sphere)) return;

	UKismetMathLibrary::VLerp(Grid_Sphere->GetComponentLocation(), LastTracePos, 10);

	Grid_Sphere->SetWorldLocation(LastTracePos);

	
}


void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_Left");

	StringHandAttach = true;

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}
}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");

	StringHandAttach = false;

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}


}

void ACAttachment_Bow::MouseWheelAxisValue(float AxisValue)
{
	
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}

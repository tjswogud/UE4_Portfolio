#include "Character/CPlayer.h"

#include <string>

#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CExecuteComponent.h"
#include "Component/CMontagesComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CTargetComponent.h"
#include "Component/CRopeActionComponent.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

ACPlayer::ACPlayer() : AHumanType()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCExecuteComponent>(this, &Execute, "Execute");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Targeting, "Targeting");
	CHelpers::CreateActorComponent<UCRopeActionComponent>(this, &RopeActionComponent, "RopeAction");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &FeetComponent, "FeetComponent");

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateComponent<USphereComponent>(this, &Climbing_Sphere, "Climbing_Sphere", (USceneComponent*)GetCapsuleComponent());
	CHelpers::CreateComponent<USphereComponent>(this, &Climbing_Sphere2, "Climbing_Sphere2", (USceneComponent*)GetCapsuleComponent());

	CHelpers::CreateComponent<UWidgetComponent>(this, &RopeAim, "RopeAim", (USceneComponent*)GetCapsuleComponent());
	CHelpers::CreateComponent<UWidgetComponent>(this, &GroggyBar, "GroggyBar", (USceneComponent*)GetCapsuleComponent());

	CHelpers::CreateComponent<UWidgetComponent>(this, &SwordSkillWidget, "SwordSkillWidget", (USceneComponent*)GetCapsuleComponent());
	CHelpers::CreateComponent<UWidgetComponent>(this, &BowSkillWidget, "BowSkillWidget", (USceneComponent*)GetCapsuleComponent());
	CHelpers::CreateComponent<UWidgetComponent>(this, &BossClearWidget, "BossClearWidget", (USceneComponent*)GetCapsuleComponent());


	SwordSkillWidget->SetVisibility(false);
	BowSkillWidget->SetVisibility(false);
	BossClearWidget->SetVisibility(false);
	RopeAim->SetVisibility(false);

	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "Skeleton'/Game/Player/Mesh/SK_Fallen_Samurai_Skeleton.SK_Fallen_Samurai_Skeleton'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimClass(animInstance);


	SpringArm->SetRelativeLocation(FVector(0, 0, 200));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 400;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;



	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	HealthBar->SetVisibility(false);
	GroggyBar->SetVisibility(false);

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);


	HealthBar->GetWidget()->AddToViewport();
}

void ACPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

		if (CameraManager)
		{
			CameraLocation = CameraManager->GetCameraLocation();
			CameraRotator = CameraManager->GetCameraRotation();
		}
	}


	if(Status->CurGroggy > 0)
	{
		GroggyBar->GetWidget()->AddToViewport();
	}
	else
	{
		GroggyBar->GetWidget()->RemoveFromViewport();
	}


	if(Status->CurGroggy > 0 && State->IsIdleMode())
	{
		Status->CurGroggy = Status->CurGroggy - 20 * DeltaSeconds;
	}
	else if(Status->CurGroggy > 0 && State->IsActionMode())
	{
		Status->CurGroggy = Status->CurGroggy - 5 * DeltaSeconds;
	}

	FHitResult HitResult;
	Landed(HitResult);

	switch (Status->GetPlaceType()) {
		case EPlaceType::OnGround: break;
	case EPlaceType::OnLand: break;
	case EPlaceType::OnWall: break;
	case EPlaceType::InAir: 
			if(State->IsIdleMode())
			{
				
				Montages->PlayAnimMontage(EStateType::Jump, 2);
			}
			
			break;
	default: ;
	}

	if(GetCharacterMovement()->GetGroundMovementMode() == EMovementMode::MOVE_Falling)
	{
		Montages->PlayAnimMontage(EStateType::Jump, 2);
	}



}

void ACPlayer::Dodge()
{

	Super::Dodge();
	

}

void ACPlayer::RopeAction()
{
	switch (Status->GetPlaceType()) {
	case EPlaceType::OnLand: 
		Montages->PlayAnimMontage(EStateType::RopeAction, 0);

		break;
	case EPlaceType::OnWall: break;
	case EPlaceType::InAir: 
		Montages->PlayAnimMontage(EStateType::RopeAction, 1);
		break;
	default: ;
	}
}

void ACPlayer::End_RopeAction()
{
	SetEndState();

	RopeActionComponent->GetRope()->Active = false;
	RopeActionComponent->IsActivate = false;

	State->CurJumpCount = 1;
	State->SetJumpMode();


}

void ACPlayer::Sneak()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	Movement->SetSpeed(ESpeedType::Sneak);

	State->SetSneakMode();
}

void ACPlayer::Hide()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	State->SetHideMode();
}

void ACPlayer::MainAction()
{


	if(!!Weapons->GetDoAction())
	{
		if(Execute->CheckCanAirExecute())
		{
			Execute->PlayAirExecution();
		}
		else if(Execute->CheckCanGroundExecute())
		{
			Execute->PlayGroundExecution();
		}
		else
		{
			CheckNull(Weapons);
			CheckNull(Weapons->GetDoAction());

			if(Status->GetPlaceType() == EPlaceType::InAir)
			{
			
				Weapons->JumpAction();
			}
			else if(GetCharacterMovement()->MaxWalkSpeed == Movement->GetSprintSpeed())
			{
				Weapons->DashAction();
			}
			else
			{
				Weapons->DoAction();
			}

		}
	}

}

void ACPlayer::DodgeAction()
{
	if (State->IsHittedMode() || State->IsExecuteMode() || State->IsDeadMode() || State->IsGroggyMode() || State->IsDodgeMode())
	{

		return;
	}
		


	State->SetDodgeMode();

}

void ACPlayer::GetMouseWheelAxis(float InAxisValue)
{
	MouseWheelAxis = InAxisValue;
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                           AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACPlayer::Hitted()
{
	Super::Hitted();

	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		if (Status->IsGroggy() && !Status->CheckGroggy())
		{

			Status->ToggleGroggy();
			State->SetGroggyMode();
		}
		else
		{
			data->PlayMontage(this);
			data->PlayHitStop(GetWorld());

		}

		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());



		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CHelpers::GetVector2D(start), CHelpers::GetVector2D(target)));
		}
	}


	if (Status->IsDead())
	{
		State->SetDeadMode();

		return;
	}




	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

}

void ACPlayer::End_Hitted()
{
	Super::End_Hitted();

	if (Status->CheckGroggy())
	{
		State->SetGroggyMode();
	}
	else
	{
		State->SetIdleMode();
	}
}

void ACPlayer::End_Dodge()
{
	Super::End_Dodge();
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("MouseWheel", this, &ACPlayer::GetMouseWheelAxis);

	PlayerInputComponent->BindAxis("ChangeTarget", Targeting, &UCTargetComponent::OnChangeTargeting);


	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::DodgeAction); //Dodge
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Repeat, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Sneak", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSneak);
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, Targeting, &UCTargetComponent::ToggleTargeting);


	
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapons, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapons, &UCWeaponComponent::SetBowMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::MainAction);
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, this, &ACPlayer::Skill1);
	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Pressed, this, &ACPlayer::Skill2);
	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Pressed, this, &ACPlayer::Skill3);
	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Pressed, this, &ACPlayer::Skill4);


	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, Weapons, &UCWeaponComponent::SubAction_Pressed);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapons, &UCWeaponComponent::SubAction_Released);

	PlayerInputComponent->BindAction("RopeAction", EInputEvent::IE_Pressed, RopeActionComponent, &UCRopeActionComponent::BeginAim);
	PlayerInputComponent->BindAction("RopeAction", EInputEvent::IE_Released, RopeActionComponent, &UCRopeActionComponent::EndAim);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, State, &UCStateComponent::SetJumpMode);

	

}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: 
		Hitted(); break;
	case EStateType::Dead: 
		Super::Dead(); break;
	case EStateType::Dodge:	Dodge();	break;
	case EStateType::Idle:  
		Idle(); break;
	case EStateType::Equip: 
		
		break;
	case EStateType::Action: 
		

		break;
	case EStateType::Ground:  

		
		Super::Ground(); break;
	case EStateType::Rise: 
		
		
		Rise(); break;
	case EStateType::Sneak:  
		
		
		break;
	case EStateType::Hide: break;
	case EStateType::Execute: 
		
		Super::Execute(); break;
	case EStateType::Max: break;
	case EStateType::Groggy:  
		
		
		Super::Groggy(); break;
	case EStateType::Parried: 
		
		
		break;
	case EStateType::Jump: 
		
		
		Jump(); break;
	case EStateType::RopeAction: 
		
		
		RopeAction(); break;
	default: ;
	}
}

void ACPlayer::Skill1()
{
	Movement->DisableControlRotation();

	skillIndex = 0;

	CurRes -= 15;
	if(CurRes < 0)
	{
		CurRes = 0;
		return;
	}

	UseSkill(skillIndex);
}

void ACPlayer::Skill2()
{
	Movement->DisableControlRotation();

	CurRes -= 10;
	if (CurRes < 0)
	{
		CurRes = 0;
		return;
	}

	skillIndex = 1;
	UseSkill(skillIndex);
}

void ACPlayer::Skill3()
{
	CurRes -= 15;
	if (CurRes < 0)
	{
		CurRes = 0;
		return;
	}

	if(Weapons->IsBowMode())
	{
		Cast<ACAttachment_Bow>(Weapons->GetAttachment())->ShowTrajectory = true;
	}
	

	skillIndex = 2;
	UseSkill(skillIndex);
}

void ACPlayer::Skill4()
{
	CurRes -= 5;
	if (CurRes < 0)
	{
		CurRes = 0;
		return;
	}

	skillIndex = 3;
	UseSkill(skillIndex);
}

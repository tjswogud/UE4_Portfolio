#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "CEnemy_AIController.generated.h"



UCLASS()
class MYPROJECT_06_API ACEnemy_AIController : public AAIController
{
	GENERATED_BODY()


public:
	ACEnemy_AIController();
	virtual void OnPossess(APawn* InPawn) override;

public:
	static const FName Target;
	static const FName Behavior;
	static const FName EQS_Location;
	static const FName TargetLocation;

public:
	void RunAI();
	void StopAI();

	

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		class UAISenseConfig_Sight* SightConfig;


public:
	UFUNCTION()
		void OnUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
		void SetPerceptionSystem();

	//AI Perception º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 700.f;


};

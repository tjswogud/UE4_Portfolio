#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CEnemy_AIController_Range.generated.h"



UCLASS()
class MYPROJECT_06_API ACEnemy_AIController_Range : public AAIController
{
	GENERATED_BODY()

public:
	ACEnemy_AIController_Range();
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
		float AISightRadius = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 1000.f;


};

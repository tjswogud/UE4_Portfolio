#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CEnemy_AIController_Boss.generated.h"


UCLASS()
class MYPROJECT_06_API ACEnemy_AIController_Boss : public AAIController
{
	GENERATED_BODY()


public:
	ACEnemy_AIController_Boss();
	virtual void OnPossess(APawn* InPawn) override;

public:
	static const FName Target;
	static const FName Behavior;
	static const FName EQS_Location;
	static const FName TargetLocation;

public:

	UFUNCTION(BlueprintCallable)
		void RunAI();

	UFUNCTION(BlueprintCallable)
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
		float AISightRadius = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 1000.f;

};

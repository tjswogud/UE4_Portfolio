#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CANS_CollisionForExec.generated.h"

UCLASS()
class MYPROJECT_06_API UCANS_CollisionForExec : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Percentage = 0.0f;

	UPROPERTY(EditAnywhere)
		bool IsLastAtk = false;

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	

};

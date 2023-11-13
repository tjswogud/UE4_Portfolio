#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/CFeetComponent.h"
#include "Component/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class MYPROJECT_06_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		bool bBow_Aiming;
protected:




protected:
	UPROPERTY(BlueprintReadWrite, Category = "Groggy")
		bool IsBlocking = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		bool bFeet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		FFeetData FeetData;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType = EWeaponType::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	class AHumanType* OwnerCharacter;
	class UCWeaponComponent* Weapon;

private:
	FRotator PrevRotation;

};

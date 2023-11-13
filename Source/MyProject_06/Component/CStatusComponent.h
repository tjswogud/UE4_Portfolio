#pragma once

#include "CoreMinimal.h"
#include "Global.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM()
enum class EPlaceType : uint8
{
	OnGround = 0, OnLand, OnWall, InAir
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_06_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Groggy")
		float MaxGroggy = 100;

	UPROPERTY(EditAnywhere, Category = "Groggy")
		bool OnGroggy = false;


	UPROPERTY(EditAnywhere, Category = "Groggy")
		float CurGroggy = 0;

	UPROPERTY(EditAnywhere, Category = "Health")
		float CurHealth = 0;

private:
	
	UPROPERTY(EditAnywhere, Category = "PlaceType")
		EPlaceType InPlaceType = EPlaceType::OnLand;
public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHealth() { return CurHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxGroggy() { return MaxGroggy; }


	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() { return CurHealth <= 0.0f; }


public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetGroggy() { return CurGroggy; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGroggy() { return CurGroggy >= MaxGroggy; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CheckGroggy() { return OnGroggy; }


	void ToggleGroggy();

	FORCEINLINE void SetPlaceType(EPlaceType place) { InPlaceType = place; }
	FORCEINLINE EPlaceType GetPlaceType() { return InPlaceType; }
public:	
	UCStatusComponent();

public:
	

protected:
	virtual void BeginPlay() override;

public:
	void Damage(float InHealth, float InGroggy = 5);

private:
	class AHumanType* Owner;



};

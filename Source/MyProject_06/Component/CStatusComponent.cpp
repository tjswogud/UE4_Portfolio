#include "Component/CStatusComponent.h"
#include "Global.h"
#include "Character/HumanType.h" 



void UCStatusComponent::ToggleGroggy()
{
	if(OnGroggy)
	{
		OnGroggy = false;
		CurGroggy = 0.0f;
		Owner->GetState()->SetIdleMode();
	}
	else
	{
		OnGroggy = true;
		Owner->GetState()->SetGroggyMode();
	}

}

UCStatusComponent::UCStatusComponent()
{

}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AHumanType>(GetOwner());

	CurHealth = MaxHealth;
	CurGroggy = 0.0f;
}

void UCStatusComponent::Damage(float InHealth, float InGroggy)
{
	CurHealth += (InHealth * -1.0f);
	CurHealth = FMath::Clamp(CurHealth, 0.0f, MaxHealth);

	CurGroggy += (InGroggy);
	CurGroggy = FMath::Clamp(CurGroggy, 0.0f, MaxGroggy);
}




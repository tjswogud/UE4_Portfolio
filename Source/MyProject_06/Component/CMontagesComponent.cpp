#include "Component/CMontagesComponent.h"

#include "CStatusComponent.h"
#include "Global.h"
#include "Character/HumanType.h"	

UCMontagesComponent::UCMontagesComponent()
{


}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AHumanType>(GetOwner());

	if (DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		
	}
	else
	{
		TArray<FMontagesData*> datas;
		DataTable->GetAllRows<FMontagesData>("", datas);

		for (FMontagesData* data : datas)
		{
			Datas.Push(*data);
		}

	}



	//
	if (LandHitDataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

	
	}
	else
	{
		TArray<FHitMontagesData*> LHdatas;
		LandHitDataTable->GetAllRows<FHitMontagesData>("", LHdatas);

		for (FHitMontagesData* data : LHdatas)
		{
			LandHittedData.Push(*data);
		}
	}

	//

	if (AirHitDataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");


	}
	else
	{
		TArray<FHitMontagesData*> AHdatas;
		AirHitDataTable->GetAllRows<FHitMontagesData>("", AHdatas);

		for (FHitMontagesData* data : AHdatas)
		{
			AirHittedData.Push(*data);
		}
	}

	//
	if (ExecuteDataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

	
	}
	else
	{
		TArray<FExecuteData*> EXdatas;
		ExecuteDataTable->GetAllRows<FExecuteData>("", EXdatas);

		for (FExecuteData* data : EXdatas)
		{
			ExecuteDatas.Push(*data);
		}
	}
}

void UCMontagesComponent::PlayDodgeMode()
{
	if (Owner->GetState()->GetPrevType() == EStateType::Ground)
	{


		PlayAnimMontage(EStateType::Dodge, (int)Owner->Direction + 4);
	}
	else
	{

		PlayAnimMontage(EStateType::Dodge, (uint8)Owner->Direction);

	}


}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayGroundMode()
{
	if (Owner->GetState()->IsFront)
	{
		PlayAnimMontage(EStateType::Ground);
	}
	else
	{
		PlayAnimMontage(EStateType::Ground, 1);
	}
}

void UCMontagesComponent::PlayRiseMode()
{
	if(Owner->GetState()->IsFront)
	{
		PlayAnimMontage(EStateType::Rise);
	}
	else
	{
		PlayAnimMontage(EStateType::Rise,1);
	}
}

void UCMontagesComponent::PlayExecuteMode(EWeaponType InWeaponType, EExecutePlace InExecutePlace, int index, bool IsSneak, FName Name)
{
	Owner->GetState()->SetExecuteMode();

	for(int i = 0; i < ExecuteDatas.Num(); i++)
	{
		if(ExecuteDatas[i].WeaponType == InWeaponType && ExecuteDatas[i].ExecutePlace == InExecutePlace && ExecuteDatas[i].index == index && ExecuteDatas[i].IsSneak == IsSneak)
		{
			if(Name == NAME_None)
			{
				Owner->PlayAnimMontage(ExecuteDatas[i].Montage, ExecuteDatas[i].PlayRate);
			}
			else
			{
				Owner->PlayAnimMontage(ExecuteDatas[i].Montage, ExecuteDatas[i].PlayRate, Name);
			}
		}
	}
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType, int Index)
{
	CheckNull(Owner);

	//switch (InType)
	//{
	//case EStateType::Idle: Owner->GetState()->SetIdleMode(); break;
	//case EStateType::Dodge: Owner->GetState()->SetDodgeMode(); break;
	//case EStateType::Equip:	Owner->GetState()->SetEquipMode(); break;
	//case EStateType::Hitted: Owner->GetState()->SetHittedMode();  break;
	//case EStateType::Dead: Owner->GetState()->SetDeadMode(); break;
	//case EStateType::Action: Owner->GetState()->SetActionMode(); break;
	//case EStateType::Ground: Owner->GetState()->SetGroundMode(); break;
	//case EStateType::Rise: Owner->GetState()->SetRiseMode(); break;
	//case EStateType::Sneak: Owner->GetState()->SetSneakMode(); break;
	//case EStateType::Hide: Owner->GetState()->SetHideMode(); break;
	//case EStateType::Groggy: Owner->GetState()->SetGroggyMode(); break;
	//case EStateType::Execute: Owner->GetState()->SetExecuteMode(); break;
	//case EStateType::Parried: Owner->GetState()->SetParriedMode(); break;
	//case EStateType::Max: break;
	//default: ;
	//}


	FMontagesData* data = nullptr;
 
	for(int i = 0; i < Datas.Num(); i++)
	{
		if(Datas[i].Type == InType && Datas[i].Index == Index)
		{
			data = &Datas[i];
		}
	}

	

	if (data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	Owner->PlayAnimMontage(data->Montage, data->PlayRate);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType, int Index, FName StartSelectionName)
{
	CheckNull(Owner);

	FMontagesData* data = nullptr;

	for (int i = 0; i < Datas.Num(); i++)
	{
		if (Datas[i].Type == InType && Datas[i].Index == Index)
		{
			data = &Datas[i];
		}
	}



	if (data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	Owner->PlayAnimMontage(data->Montage, data->PlayRate, StartSelectionName);
	
}

void UCMontagesComponent::PlayRandomIndexOfAnim(EStateType InType)
{
	int count = 0;

	for (FMontagesData Data : Datas)
	{
		if(Data.Type == InType)
		{
			count++;
		}
	}

	PlayAnimMontage(InType, FMath::RandRange(0, count - 1));

}

void UCMontagesComponent::PlayHIttedAnim(EHittedType InType, int Index)
{
	if(Owner->GetStatus()->GetPlaceType() == EPlaceType::InAir)
	{
		if (AirHittedData.Num() > 0)
		{
			if (Index == -1)
			{
				for (int i = 0; i < AirHittedData.Num(); i++)
				{
					if (AirHittedData[i].HittedType == InType)
					{
						Owner->PlayAnimMontage(AirHittedData[i].Montage, AirHittedData[i].PlayRate);
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < AirHittedData.Num(); i++)
				{
					if (AirHittedData[i].HittedType == InType && AirHittedData[i].Index == Index)
					{
						Owner->PlayAnimMontage(AirHittedData[i].Montage, AirHittedData[i].PlayRate);
						break;
					}
				}
			}

		}
	}
	else
	{
		if (LandHittedData.Num() > 0)
		{
			if (Index == -1)
			{
				for (int i = 0; i < LandHittedData.Num(); i++)
				{
					if (LandHittedData[i].HittedType == InType)
					{
						Owner->PlayAnimMontage(LandHittedData[i].Montage, LandHittedData[i].PlayRate);
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < LandHittedData.Num(); i++)
				{
					if (LandHittedData[i].HittedType == InType && LandHittedData[i].Index == Index)
					{
						Owner->PlayAnimMontage(LandHittedData[i].Montage, LandHittedData[i].PlayRate);
						break;
					}
				}
			}

		}
	}

	


}



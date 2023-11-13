#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AddOn/CGhostTrail.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"

#define CheckTrue(x) { if((x) == true) return; }
#define CheckTrueResult(x, y) { if((x) == true) return y; }

#define CheckFalse(x) { if((x) == false) return;}
#define CheckFalseResult(x, y) { if((x) == false) return y;}

#define CheckNull(x) { if((x) == nullptr) return;}
#define CheckNullResult(x, y) { if((x) == nullptr) return y;}




#define CreateTextRender() \
{  \
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root); \
	Text->SetRelativeLocation(FVector(0, 0, 100)); \
	Text->SetRelativeRotation(FRotator(0, 180, 0)); \
	Text->SetRelativeScale3D(FVector(2)); \
	Text->TextRenderColor = FColor::Red; \
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center; \
	Text->Text = FText::FromString(GetName().Replace(L"Default__", L"")); \
}

class MYPROJECT_06_API CHelpers
{
public:
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent, InSocketName);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static T* FindActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<T>())
				return Cast<T>(actor);
		}

		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		OutActors.Empty();

		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<T>())
				OutActors.Add(Cast<T>(actor));
		}
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static T* GetComponent(AActor* InActor, const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);
		
		for (T* component : components)
		{
			if (component->GetName() == InName)
				return component;
		}

		return nullptr;
	}

	static void AttachTo(AActor* InActor, USceneComponent* InParent, const FName& InSocketName)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}

	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform, UMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None, UParticleSystemComponent* OutPSC = nullptr, UNiagaraComponent* OutNC = nullptr )
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InAsset);

		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (!!InMesh)
		{
			if (!!particle)
			{
				OutPSC = UGameplayStatics::SpawnEmitterAttached(particle, InMesh, InSocketName, location, rotation, scale);

				return;
			}

			if (!!niagara)
			{
				OutNC = UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InMesh, InSocketName, location, rotation, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

				return;
			}

			return;
		}

		if (!!particle)
		{
			OutPSC =UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);

			return;
		}

		if (!!niagara)
		{
			OutNC =	UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotation, scale);

			return;
		}

	}

	

	static FVector GetVector2D(FVector inVector)
	{
		return FVector(inVector.X, inVector.Y, 0);
	}

	static void Delay(UWorld* World, float DelayTime)
	{
		// 딜레이 시간 (초 단위)
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, [=]() {
			// 딜레이 후 실행할 코드 작성
		}, DelayTime, false);

	}

	static ACGhostTrail* Play_GhostTrail(TSubclassOf<ACGhostTrail>& InClass, class ACharacter* InOwner)
	{
		CheckNullResult(InClass, nullptr);
		CheckNullResult(InOwner, nullptr);


		FActorSpawnParameters params;
		params.Owner = InOwner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector location = InOwner->GetActorLocation();
		location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();


		FTransform transform;
		transform.SetTranslation(location);

		return InOwner->GetWorld()->SpawnActor<ACGhostTrail>(InClass, transform, params);
	}
};

#include "EffectManager.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

UEffectManager* UEffectManager::Instance = nullptr;

UEffectManager* UEffectManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UEffectManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

void UEffectManager::ApplyEffect(AActor* TargetActor, TSubclassOf<UActorComponent> EffectType)
{
	if (TargetActor && EffectType)
	{
		UActorComponent* NewEffect = NewObject<UActorComponent>(TargetActor, EffectType);
		if (NewEffect)
		{
			NewEffect->RegisterComponent();
		}
	}
}

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectManager.generated.h"

UCLASS()
class INFECTEDCITY_API UEffectManager : public UObject
{
	GENERATED_BODY()

public:
	static UEffectManager* Get();

	void ApplyEffect(AActor* TargetActor, TSubclassOf<UActorComponent> EffectType);

protected:
	static UEffectManager* Instance;
};

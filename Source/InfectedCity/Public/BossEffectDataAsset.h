#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossEffectDataAsset.generated.h"

UCLASS()
class INFECTEDCITY_API UBossEffectDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Effects")
    TMap<FName, UParticleSystem*> EffectMap;

    UPROPERTY(EditAnywhere, Category = "Sounds")
    TMap<FName, USoundBase*> SoundMap;
};

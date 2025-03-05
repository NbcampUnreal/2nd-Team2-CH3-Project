// BossEffectManager.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BossEffectManager.generated.h"

UCLASS()
class INFECTEDCITY_API UBossEffectManager : public UObject
{
    GENERATED_BODY()

public:
    static UBossEffectManager* GetInstance();

    void Initialize(class UBossEffectDataAsset* DataAsset);

    void PlayEffect(UWorld* World, FName EffectName, FVector Location);
    void PlaySound(UWorld* World, FName SoundName, FVector Location);

private:
    static UBossEffectManager* Instance;

    TMap<FName, UParticleSystem*> EffectMap;
    TMap<FName, USoundBase*> SoundMap;
};
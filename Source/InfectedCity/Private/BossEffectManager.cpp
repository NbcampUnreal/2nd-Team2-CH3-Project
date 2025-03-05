#include "BossEffectManager.h"
#include "BossEffectDataAsset.h"
#include "Kismet/GameplayStatics.h"

UBossEffectManager* UBossEffectManager::Instance = nullptr;

UBossEffectManager* UBossEffectManager::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UBossEffectManager>();
        Instance->AddToRoot();
    }

    return Instance;
}

void UBossEffectManager::Initialize(UBossEffectDataAsset* DataAsset)
{
    if (DataAsset)
    {
        EffectMap = DataAsset->EffectMap;
        SoundMap = DataAsset->SoundMap;
    }
}

void UBossEffectManager::PlayEffect(UWorld* World, FName EffectName, FVector Location)
{
    if (EffectMap.Contains(EffectName))
    {

        UGameplayStatics::SpawnEmitterAtLocation(World, EffectMap[EffectName], Location);
    }
}

void UBossEffectManager::PlaySound(UWorld* World, FName SoundName, FVector Location)
{
    if (SoundMap.Contains(SoundName))
    {
        UGameplayStatics::PlaySoundAtLocation(World, SoundMap[SoundName], Location);
    }
}

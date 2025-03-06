#include "BossEffectManager.h"
#include "BossEffectDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
        NiagaraEffectMap = DataAsset->NiagaraEffectMap;
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

void UBossEffectManager::PlayNiagaraEffect(UWorld* World, FName EffectName, FVector Location)
{
    if (NiagaraEffectMap.Contains(EffectName))
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, NiagaraEffectMap[EffectName], Location);
    }
}

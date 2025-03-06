#include "MobSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

AMobSpawner::AMobSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 설정
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // 박스 콜라이더 생성
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    SpawnArea->SetupAttachment(Root);
}

void AMobSpawner::BeginPlay()
{
    Super::BeginPlay();

    SpawnMobs();
}

void AMobSpawner::SpawnMobs()
{
    if (!MobClass) return; // MobClass가 설정되지 않았으면 스폰하지 않음

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        FVector SpawnLocation = GetActorLocation();

        // 박스 콜라이더 내에서 랜덤 위치 설정
        FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
        FVector RandomOffset = FVector(
            FMath::RandRange(-BoxExtent.X, BoxExtent.X),
            FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
            0.0f
        );

        SpawnLocation += RandomOffset;

        // 액터 스폰
        FActorSpawnParameters SpawnParams;
        World->SpawnActor<AActor>(MobClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    }
}

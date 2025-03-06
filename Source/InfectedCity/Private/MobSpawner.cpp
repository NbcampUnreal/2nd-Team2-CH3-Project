#include "MobSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

AMobSpawner::AMobSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // ��Ʈ ������Ʈ ����
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // �ڽ� �ݶ��̴� ����
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
    if (!MobClass) return; // MobClass�� �������� �ʾ����� �������� ����

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        FVector SpawnLocation = GetActorLocation();

        // �ڽ� �ݶ��̴� ������ ���� ��ġ ����
        FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
        FVector RandomOffset = FVector(
            FMath::RandRange(-BoxExtent.X, BoxExtent.X),
            FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
            0.0f
        );

        SpawnLocation += RandomOffset;

        // ���� ����
        FActorSpawnParameters SpawnParams;
        World->SpawnActor<AActor>(MobClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    }
}

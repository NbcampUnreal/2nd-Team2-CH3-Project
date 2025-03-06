#include "ChaseManSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

AChaseManSpawner::AChaseManSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // ��Ʈ ������Ʈ ����
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // �ڽ� �ݶ��̴� ����
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    SpawnArea->SetupAttachment(Root);
}

void AChaseManSpawner::BeginPlay()
{
    Super::BeginPlay();

    //SpawnMobs();
}

void AChaseManSpawner::SpawnMobs()
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
        World->SpawnActor<AGN_ChaseMan>(MobClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    }
}

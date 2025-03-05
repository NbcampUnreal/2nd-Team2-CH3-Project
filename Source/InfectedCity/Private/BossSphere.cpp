#include "BossSphere.h"
#include "Boss.h"
#include "Components/StaticMeshComponent.h"

ABossSphere::ABossSphere()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ABossSphere::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ABossSphere Spawned!"));
}

void ABossSphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Boss && Boss->CurrentPattern == EBossPattern::Idle)
    {
        CurrentAngle += OrbitSpeed * DeltaTime;
        float AngleRad = FMath::DegreesToRadians(CurrentAngle);

        float Radius = 100.0f;
        FVector NewLocation = Boss->GetActorLocation() + FVector(FMath::Cos(AngleRad) * Radius, FMath::Sin(AngleRad) * Radius, 0);
        SetActorLocation(NewLocation);
    }
}

void ABossSphere::OnPatternChanged(EBossPattern NewPattern)
{
    UE_LOG(LogTemp, Warning, TEXT("ABossSphere pattern change: %d"), (int32)NewPattern);
    //if (NewPattern == EBossPattern::Laser)
    //{
    //    SetActorLocation(GetOwner()->GetActorLocation());
    //}
}

void ABossSphere::SetBoss(ABoss* InBoss)
{
    Boss = InBoss;
}

//if (BossActor)
//{
//    float Time = GetWorld()->GetTimeSeconds();
//    float Radius = 300.0f;
//    float Speed = 1.5f;
//    FVector BossLocation = BossActor->GetActorLocation();
//    FVector NewLocation = BossLocation + FVector(FMath::Cos(Time * Speed) * Radius, FMath::Sin(Time * Speed) * Radius, 50.0f);
//    SetActorLocation(NewLocation);
//}
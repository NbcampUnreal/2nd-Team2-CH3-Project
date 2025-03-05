#include "BossSphere.h"
#include "Boss.h"
#include "Components/StaticMeshComponent.h"

ABossSphere::ABossSphere()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    LocalTransform = FTransform::Identity;
}

void ABossSphere::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ABossSphere Spawned!"));
}

void ABossSphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Boss == nullptr) return;

    if (Boss->CurrentPattern == EBossPattern::Idle)
    {
        IdleAnimation(DeltaTime);
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

void ABossSphere::SetBoss(ABoss* InitialBoss, int32 InitialUniqueId ,float InitialAngle, float* InitialOrbitSpeed, float* InitialOrbitRadius)
{
    Boss            = InitialBoss;
    UniquId         = InitialUniqueId;
    CurrentAngle    = InitialAngle;
    OrbitSpeed      = InitialOrbitSpeed;
    OrbitRadius     = InitialOrbitRadius;

    LocalTransform = GetTransform();

    float AngleRad = FMath::DegreesToRadians(CurrentAngle);

    FVector NewLocation = FVector(FMath::Cos(AngleRad) * (*OrbitRadius), FMath::Sin(AngleRad) * (*OrbitRadius), 0);

    LocalTransform.SetLocation(NewLocation);
}

void ABossSphere::IdleAnimation(float DeltaTime)
{
    LocalTransform.SetLocation(FVector(LocalTransform.GetLocation().X + DeltaTime * 10.f, 0.f, 0.f));
    SetActorTransform(LocalTransform * Boss->Transform);
}



#include "Boss.h"
#include "BossSphere.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"  

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    RootComponent = SkeletalMeshComponent;
    LocationOffset = FVector(0.0f, 0.0f, 600.0f);
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();

    RandomRotationAxis = FVector(FMath::FRandRange(-1.0f, 1.0f),
                                 FMath::FRandRange(-1.0f, 1.0f),
                                 FMath::FRandRange(-1.0f, 1.0f)).GetSafeNormal();

    AxisChangeTime = 1.0f;
    CurrentAxisTime = 0.0f;

    Transform = GetActorTransform();
    Transform.SetLocation(Transform.GetLocation() + LocationOffset);

    if (IdleAnimationAsset)
    {
        SkeletalMeshComponent->PlayAnimation(IdleAnimationAsset, true);
    }

    SpawnSpheres();

    /* ¿·Ω√ */
    //GetWorldTimerManager().SetTimerForNextTick(this, &ABoss::ChangePattern);
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentPattern == EBossPattern::Idle)
    {
        IdleAnimation(DeltaTime);
    }
}

void ABoss::SpawnSpheres()
{
    if (!BossSphereClass) 
        return;

    float Radius = 100.0f;
    float AngleStep = 360.0f / NumSpheres;

    for (int i = 0; i < NumSpheres; i++)
    {
        float Angle = FMath::DegreesToRadians(AngleStep * i);
        FVector Offset = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0);
        FVector SpawnLocation = FVector::ZeroVector;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ABossSphere* NewSphere = GetWorld()->SpawnActor<ABossSphere>(BossSphereClass, SpawnLocation, SpawnRotation);
        if (NewSphere)
        {
            NewSphere->SetBoss(this, i, AngleStep * i, &OribitSpeed, &OribitRadius);
            RegisterObserver(NewSphere);
        }
    }
}

void ABoss::RegisterObserver(TScriptInterface<IBossObserverInterface> Observer)
{
    Observers.Add(Observer);
}

void ABoss::UnregisterObserver(TScriptInterface<IBossObserverInterface> Observer)
{
    Observers.Remove(Observer);
}

void ABoss::NotifyObservers()
{
    for (TScriptInterface<IBossObserverInterface> Observer : Observers)
    {
        if (Observer)
        {
            Observer->OnPatternChanged(CurrentPattern);
        }
    }
}

void ABoss::ChangePattern()
{
    CurrentPattern = (CurrentPattern == EBossPattern::Orbit) ? EBossPattern::Laser : EBossPattern::Orbit;
    NotifyObservers();
    GetWorldTimerManager().SetTimerForNextTick(this, &ABoss::ChangePattern);
}

void ABoss::IdleAnimation(float DeltaTime)
{
    CurrentAxisTime += DeltaTime;
    if (CurrentAxisTime >= AxisChangeTime)
    {
        FVector NewRandomAxis = FVector(FMath::FRandRange(-1.0f, 1.0f),
            FMath::FRandRange(-1.0f, 1.0f),
            FMath::FRandRange(-1.0f, 1.0f)).GetSafeNormal();

        RandomRotationAxis = FMath::VInterpTo(RandomRotationAxis, NewRandomAxis, DeltaTime, 1.0f);

        OribitSpeed = FMath::FRandRange(50.f, 500.f);
        CurrentAxisTime = 0.0f;
    }


    float DeltaRotation = OribitSpeed * DeltaTime;

    Transform.ConcatenateRotation(FQuat(RandomRotationAxis, FMath::DegreesToRadians(DeltaRotation)));
}

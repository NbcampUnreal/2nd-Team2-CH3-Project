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
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();

    if (IdleAnimation)
    {
        SkeletalMeshComponent->PlayAnimation(IdleAnimation, true);
    }

    SpawnSpheres();
    //GetWorldTimerManager().SetTimerForNextTick(this, &ABoss::ChangePattern);
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
        FVector SpawnLocation = GetActorLocation() + Offset;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ABossSphere* NewSphere = GetWorld()->SpawnActor<ABossSphere>(BossSphereClass, SpawnLocation, SpawnRotation);
        if (NewSphere)
        {
            NewSphere->SetBoss(this);
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

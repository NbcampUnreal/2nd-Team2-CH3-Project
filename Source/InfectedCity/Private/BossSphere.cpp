#include "BossSphere.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "BossEffectManager.h"
#include "Boss.h"

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

    IdleWorldTransform = LocalTransform * Boss->Transform;

    if (Boss->CurrentPattern == EBossPattern::Idle)
    {
        IdleAnimation(DeltaTime);
    }
    else if (Boss->CurrentPattern == EBossPattern::HeavyCrash)
    {
        if (bMoveToChange == true)
        {
            TargetToMove(DeltaTime);
        }
        else
        {
            HeavyCrash(DeltaTime);
        }
    }

}

void ABossSphere::OnPatternChanged(EBossPattern NewPattern)
{
    UE_LOG(LogTemp, Warning, TEXT("ABossSphere pattern change: %d"), (int32)NewPattern);

    if (NewPattern == EBossPattern::HeavyCrash)
    {
        FVector BossLocation = Boss->Transform.GetLocation();
        
        float OneSpcae = 300.f;
        TargetPoint = { BossLocation.X - (OneSpcae * 2) + (UniqueId * OneSpcae), BossLocation.Y - 300.f, BossLocation.Z };

        bMoveToChange = true;

        AccMoveTime = 0.f;
        MoveInSecond = 2.f;

        MoveStartPoint = GetActorLocation();
    }
}

void ABossSphere::IdleAnimation(float DeltaTime)
{
    SetActorTransform(IdleWorldTransform);
}

void ABossSphere::TargetToMove(float DeltaTime)
{
    AccMoveTime += DeltaTime;
    float Alpha = FMath::Clamp(AccMoveTime / MoveInSecond, 0.0f, 1.0f);

    SetActorLocation(FMath::Lerp(MoveStartPoint, TargetPoint, Alpha));

    if (Alpha >= 1.f)
    {
        Boss->MoveDoneCount++; /* 쓸지말지 고민 */

        bMoveToChange   = false;
        AccMoveTime     = 0.f;
        MoveInSecond    = 0.f;

        TargetPoint     = FVector::ZeroVector;
        MoveStartPoint  = FVector::ZeroVector;

        InitializeAfterMoveDone();
    }
}

void ABossSphere::InitializeAfterMoveDone()
{
    if (Boss->CurrentPattern == EBossPattern::HeavyCrash)
    {
        WarningAccTime = 0.f;
        WarningDuration = 3.f + UniqueId * 0.5f;

        PatternAccTime = 0.f;
        PatternDuration = 0.5f;

        bWarningFinished = false;

        OriginalLocation = GetActorLocation();
    }
}

void ABossSphere::HeavyCrash(float DeltaTime)
{
    
    if (bWarningFinished == false)
    {
        WarningAccTime += DeltaTime;

        float ShakeIntensity = 5.0f;
        float ShakeSpeed = 50.0f;

        FVector ShakeOffset;
        ShakeOffset.X = FMath::RandRange(-ShakeIntensity, ShakeIntensity);
        ShakeOffset.Y = FMath::RandRange(-ShakeIntensity, ShakeIntensity);
        ShakeOffset.Z = FMath::Sin(GetWorld()->TimeSeconds * ShakeSpeed) * ShakeIntensity;

        SetActorLocation(OriginalLocation + ShakeOffset);

        if (WarningAccTime > WarningDuration - 0.3f && TargetPoint == FVector::ZeroVector)
        {
            if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
            {
                FVector PlayerLocation = PlayerPawn->GetActorLocation();
                PlayerLocation -= FVector(0, 0, PlayerPawn->GetSimpleCollisionHalfHeight());
                TargetPoint = PlayerLocation;

                UE_LOG(LogTemp, Warning, TEXT("Lockon Player ! ID : %d"), UniqueId);
            }
        }

        if (WarningAccTime >= WarningDuration)
        {
            MoveStartPoint = GetActorLocation();
            bWarningFinished = true;
        }
    }

    if (bWarningFinished == true)
    {
        PatternAccTime += DeltaTime;

        float Alpha = FMath::Clamp(PatternAccTime / PatternDuration, 0.0f, 1.0f);

        SetActorLocation(FMath::Lerp(MoveStartPoint, TargetPoint, Alpha));

        if (Alpha >= 1.0f)
        {
            /* 이펙트 사운드 추가 */
            /* 중복 발생 여기서 제어하던가 아니면 이펙트 메니저에서 세팅값 추가하던가 */
            UBossEffectManager::GetInstance()->PlaySound(GetWorld(), TEXT("HeavyCrash_Landing"), GetActorLocation());
        }
    }
}

void ABossSphere::SetBoss(ABoss* InitialBoss, int32 InitialUniqueId, float InitialAngle, float* InitialOrbitSpeed, float* InitialOrbitRadius)
{
    Boss = InitialBoss;
    UniqueId = InitialUniqueId;
    CurrentAngle = InitialAngle;
    OrbitSpeed = InitialOrbitSpeed;
    OrbitRadius = InitialOrbitRadius;

    LocalTransform = GetTransform();

    float AngleRad = FMath::DegreesToRadians(CurrentAngle);

    FVector NewLocation = FVector(FMath::Cos(AngleRad) * (*OrbitRadius), FMath::Sin(AngleRad) * (*OrbitRadius), 0);

    LocalTransform.SetLocation(NewLocation);
}

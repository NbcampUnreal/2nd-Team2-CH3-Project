#include "BossSphere.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "BossEffectManager.h"
#include "Boss.h"

ABossSphere::ABossSphere()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetSphereRadius(100.0f);
    SphereCollider->SetCollisionProfileName(TEXT("Trigger"));

    LocalTransform = FTransform::Identity;
}

void ABossSphere::BeginPlay()
{
    Super::BeginPlay();

    /* 디버그 */
    SphereCollider->SetHiddenInGame(false);

    if (MeshComponent)
    {
        DynMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    }
}

void ABossSphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Boss == nullptr || bFinishedLock == true) return;

    IdleWorldTransform = LocalTransform * Boss->Transform;

    if (Boss->CurrentPattern == EBossPattern::Idle)
    {
        IdleAnimation(DeltaTime);
    }
    else if (Boss->CurrentPattern == EBossPattern::Lock)
    {
        if (bMoveToChange == true)
        {
            TargetPoint = IdleWorldTransform.GetLocation();
            TargetToMove(DeltaTime);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Lockon Player ! ID : %d"), UniqueId);

            bFinishedLock = true;
            Boss->MoveDoneCount++;
        }
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
    else if (Boss->CurrentPattern == EBossPattern::Beam)
    {
        IdleAnimation(DeltaTime);
    }
}

void ABossSphere::OnPatternChanged(EBossPattern NewPattern)
{
    UE_LOG(LogTemp, Warning, TEXT("ABossSphere pattern change: %d"), (int32)NewPattern);

    if (NewPattern == EBossPattern::Idle)
    {
        bFinishedLock = false;
    }
    else if (NewPattern == EBossPattern::Lock)
    {
        bFinishedLock = false;
        bMoveToChange = true;

        AccMoveTime = 0.f;
        MoveInSecond = 1.f;

        MoveStartPoint = GetActorLocation();
    }
    else if (NewPattern == EBossPattern::HeavyCrash)
    {
        FVector BossLocation = Boss->Transform.GetLocation();
        
        float OneSpcae = 300.f;
        TargetPoint = { BossLocation.X - (OneSpcae * 2) + (UniqueId * OneSpcae), BossLocation.Y - 300.f, BossLocation.Z };

        bMoveToChange = true;

        AccMoveTime = 0.f;
        MoveInSecond = 1.f;

        MoveStartPoint = GetActorLocation();
    }
    else if (NewPattern == EBossPattern::Beam)
    {
        
    }
}

void ABossSphere::IdleAnimation(float DeltaTime)
{
    SetActorTransform(IdleWorldTransform);
}

void ABossSphere::EnableTick()
{
    SetActorTickEnabled(true);
}

void ABossSphere::CustomCollisionCheck()
{
    if (Boss->CurrentPattern == EBossPattern::HeavyCrash)
    {
        if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
        {
            FVector PlayerLocation = PlayerPawn->GetActorLocation();
            PlayerLocation -= FVector(0, 0, PlayerPawn->GetSimpleCollisionHalfHeight());

            float SphereRadius = SphereCollider->GetUnscaledSphereRadius();
            float Distance = FVector::Dist(GetActorLocation(), PlayerLocation);

            if (Distance <= SphereRadius)
            {
                UE_LOG(LogTemp, Warning, TEXT("Player Collision! Distance: %f, Radius: %f"), Distance, SphereRadius);
            }
        }
    }
}

void ABossSphere::TargetToMove(float DeltaTime)
{
    AccMoveTime += DeltaTime;
    float Alpha = FMath::Clamp(AccMoveTime / MoveInSecond, 0.0f, 1.0f);

    SetActorLocation(FMath::Lerp(MoveStartPoint, TargetPoint, Alpha));

    if (Alpha >= 1.f)
    {
        bMoveToChange   = false;
        AccMoveTime     = 0.f;
        MoveInSecond    = 0.f;

        TargetPoint     = FVector::ZeroVector;
        MoveStartPoint  = FVector::ZeroVector;

        if (bPatternFinished == true)
        {
            Boss->MoveDoneCount++;
            bFinishedLock = true;
            bPatternFinished = false;
        }
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
        
        float WarningFactor = FMath::Clamp(WarningAccTime / WarningDuration, 0.0f, 1.0f);
        DynMaterialInstance->SetVectorParameterValue(TEXT("Factor"), FVector(1.f, 1.f - WarningFactor, 1.f - WarningFactor));

        if (WarningAccTime > WarningDuration - 0.3f && TargetPoint == FVector::ZeroVector)
        {
            if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
            {
                FVector PlayerLocation = PlayerPawn->GetActorLocation();
                PlayerLocation -= FVector(0, 0, PlayerPawn->GetSimpleCollisionHalfHeight());
                TargetPoint = PlayerLocation;
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

        DynMaterialInstance->SetVectorParameterValue(TEXT("Factor"), FVector(1.f, Alpha, Alpha));

        if (Alpha >= 1.0f)
        {
            /* 충돌 체크 */
            CustomCollisionCheck();

            /* 이펙트 사운드 추가 */
            UBossEffectManager::GetInstance()->PlaySound(GetWorld(), TEXT("HeavyCrash_Landing"), GetActorLocation());
            UBossEffectManager::GetInstance()->PlayEffect(GetWorld(), TEXT("HeavyCrash_Landing"), GetActorLocation());
            UBossEffectManager::GetInstance()->PlayEffect(GetWorld(), TEXT("HeavyCrash_LandingFog"), GetActorLocation());

            /* 1초 딜레이 */
            SetActorTickEnabled(false);
            GetWorldTimerManager().SetTimer(EnableTimerHandle, this, &ABossSphere::EnableTick, 1.0f, false);

            /* 원래 위치로 돌아가기 세팅 */
            FVector BossLocation = Boss->Transform.GetLocation();
            float OneSpcae = 300.f;
            TargetPoint = { BossLocation.X - (OneSpcae * 2) + (UniqueId * OneSpcae), BossLocation.Y - 300.f, BossLocation.Z };
            bMoveToChange = true;
            AccMoveTime = 0.f;
            MoveInSecond = 2.f;
            MoveStartPoint = GetActorLocation();

            /* 패턴 끝났다 */
            bPatternFinished = true;
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

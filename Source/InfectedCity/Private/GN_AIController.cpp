#include "GN_AIController.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "GN_Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"

AGN_AIController::AGN_AIController()
{
}

void AGN_AIController::BeginPlay()
{
    Super::BeginPlay();
    StartPatrolling();
}

void AGN_AIController::StartPatrolling()
{
    MoveToRandomLocation();
}

void AGN_AIController::MoveToRandomLocation()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            FNavLocation RandomPoint;
            if (NavSys->GetRandomPointInNavigableRadius(ControlledPawn->GetActorLocation(), 1000.0f, RandomPoint))
            {
                MoveToLocation(RandomPoint.Location);
            }
        }
    }

    float RandomDelay = FMath::RandRange(2.0f, 5.0f);
    GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &AGN_AIController::MoveToRandomLocation, RandomDelay, false);
}

void AGN_AIController::MoveToActor(AActor* Target)
{
    if (!IsValid(Target))
    {
        UE_LOG(LogTemp, Error, TEXT("MoveToActor() - Target이 NULL입니다!"));
        return;
    }

    FVector TargetLocation = Target->GetActorLocation();
    FVector AI_Location = GetPawn()->GetActorLocation();
    float Distance = FVector::Dist(AI_Location, TargetLocation);

    UE_LOG(LogTemp, Warning, TEXT("MoveToActor() - AI와 목표 지점 사이 거리: %f"), Distance);

    // 이동할 수 있는지 네비게이션 메시(NavMesh) 확인
    if (!GetWorld()->GetNavigationSystem())
    {
        UE_LOG(LogTemp, Error, TEXT("MoveToActor() - 네비게이션 시스템이 존재하지 않음!"));
        return;
    }

    // 이동 명령 실행
    EPathFollowingRequestResult::Type MoveResult = MoveToLocation(TargetLocation);

    switch (MoveResult)
    {
    case EPathFollowingRequestResult::Failed:
       // UE_LOG(LogTemp, Error, TEXT("MoveToActor() - 이동 실패! 네비 메시(NavMesh) 확인 필요!"));
        break;

    case EPathFollowingRequestResult::RequestSuccessful:
        //UE_LOG(LogTemp, Warning, TEXT("MoveToActor() - 이동 명령 성공! AI가 이동을 시도합니다."));
        break;

    case EPathFollowingRequestResult::AlreadyAtGoal:
        //UE_LOG(LogTemp, Warning, TEXT("MoveToActor() - 이미 목표 지점에 도착해 있음."));
        break;

    default:
        //UE_LOG(LogTemp, Warning, TEXT("MoveToActor() - 이동 요청이 접수되었지만, 즉시 완료되지 않음."));
        break;
    }
}

// AI가 계속 목표를 따라가도록 설정
void AGN_AIController::FollowTarget()
{
    if (CurTarget)
    {
        MoveToActor(CurTarget);
    }
}

void AGN_AIController::RemovePatrolling()
{
    GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
}

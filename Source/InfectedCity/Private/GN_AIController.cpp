#include "GN_AIController.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
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
		UE_LOG(LogTemp, Error, TEXT("OnSeePawn: SeenPawn is NULL!"));
		return;
	}

	if (Target)
	{
		FVector PlayerLocation = Target->GetActorLocation();
		if (Target)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player : %f"), PlayerLocation.Size());
			MoveToLocation(Target->GetActorLocation());
		}
	}
}

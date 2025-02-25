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

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AGN_AIController::MoveToRandomLocation);
}

void AGN_AIController::MoveToActor(AActor* Target)
{
	if (Target)
	{
		FVector PlayerLocation = Target->GetActorLocation();

		MoveToLocation(Target->GetActorLocation());
		
	}
}

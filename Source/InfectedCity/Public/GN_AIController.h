// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GN_AIController.generated.h"

/**
 *
 */
UCLASS()
class INFECTEDCITY_API AGN_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AGN_AIController();

protected:
	virtual void BeginPlay() override;

public:
	void StartPatrolling();
	void MoveToRandomLocation();
	void MoveToActor(AActor* Target);
	void FollowTarget();

private:
	FTimerHandle PatrolTimerHandle;

	AActor* CurTarget;
};

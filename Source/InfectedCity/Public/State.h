// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "State.generated.h"

UCLASS()
class INFECTEDCITY_API AState : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

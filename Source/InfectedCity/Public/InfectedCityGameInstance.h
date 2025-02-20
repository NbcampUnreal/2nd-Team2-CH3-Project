// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InfectedCityGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTEDCITY_API UInfectedCityGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY()
	UTextureRenderTarget2D* DepthRenderTarget;
	
	UFUNCTION()
	UTextureRenderTarget2D* GetDepthRenderTarget();
};

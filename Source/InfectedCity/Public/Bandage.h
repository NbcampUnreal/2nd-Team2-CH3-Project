// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Bandage.generated.h"

/**
 * 
 */
UCLASS()
class INFECTEDCITY_API ABandage : public AItemBase
{
	GENERATED_BODY()
	
public:

	ABandage();
public:
	void Tick(float DeltaTime) override;

};

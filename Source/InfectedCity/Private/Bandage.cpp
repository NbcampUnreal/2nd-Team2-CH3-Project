// Fill out your copyright notice in the Description page of Project Settings.


#include "Bandage.h"

ABandage::ABandage()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemType = "Bandage";
}

void ABandage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

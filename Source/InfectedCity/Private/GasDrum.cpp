#include "GasDrum.h"
#include "Components/StaticMeshComponent.h"

AGasDrum::AGasDrum()
{
    PrimaryActorTick.bCanEverTick = true;

    ItemType = "GasDrum";
}

void AGasDrum::BeginPlay()
{
    Super::BeginPlay();

}

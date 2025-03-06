#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBossPattern : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Lock UMETA(DisplayName = "Lock"),
    Beam UMETA(DisplayName = "Orbit"),
    Orbit UMETA(DisplayName = "Orbit"),
    Laser UMETA(DisplayName = "Laser"),
    Lightning UMETA(DisplayName = "Orbit"),
    HeavyCrash UMETA(DisplayName = "Orbit"),
};
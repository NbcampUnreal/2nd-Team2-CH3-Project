#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBossPattern : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Orbit UMETA(DisplayName = "Orbit"),
    Laser UMETA(DisplayName = "Laser")
};
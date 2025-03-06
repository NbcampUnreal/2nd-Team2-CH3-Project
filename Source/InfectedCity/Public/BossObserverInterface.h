#pragma once

#include "CoreMinimal.h"
#include "BossPattern.h"
#include "UObject/Interface.h"
#include "BossObserverInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBossObserverInterface : public UInterface
{
    GENERATED_BODY()
};

class INFECTEDCITY_API IBossObserverInterface
{
    GENERATED_BODY()

public:
    virtual void OnPatternChanged(EBossPattern NewPattern) = 0;
};

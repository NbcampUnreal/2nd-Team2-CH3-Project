#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GasDrum.generated.h"

UCLASS()
class INFECTEDCITY_API AGasDrum : public ABaseItem
{
	GENERATED_BODY()
	
public:
    AGasDrum();

    virtual void BeginPlay() override;
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemBase.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UItemBase : public UInterface
{
    GENERATED_BODY()
};

class INFECTEDCITY_API IItemBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void UseItem(AActor* User);
};

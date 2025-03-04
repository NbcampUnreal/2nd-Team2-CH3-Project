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
    // 아이템 사용 함수 (상속받은 클래스에서 구현)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void UseItem(AActor* User);
};

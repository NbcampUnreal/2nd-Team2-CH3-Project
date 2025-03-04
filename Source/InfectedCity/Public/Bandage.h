#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Bandage.generated.h"

UCLASS()
class INFECTEDCITY_API ABandage : public AActor, public IItemBase
{
    GENERATED_BODY()

public:
    ABandage();

    // 아이템 사용 (HP 회복)
    virtual void UseItem_Implementation(AActor* User) override;
};

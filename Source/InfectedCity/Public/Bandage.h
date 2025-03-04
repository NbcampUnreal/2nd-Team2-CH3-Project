#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Bandage.generated.h"

UCLASS()
class INFECTEDCITY_API ABandage : public ABaseItem
{
    GENERATED_BODY()

public:
    ABandage();
   
    // ������ ��� (HP ȸ��)
    virtual void UseItem_Implementation(AActor* User) override;
};

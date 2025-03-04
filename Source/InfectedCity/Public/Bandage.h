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
    
	void Tick(float DeltaTime) override;
    // ������ ��� (HP ȸ��)
    virtual void UseItem_Implementation(AActor* User) override;
};

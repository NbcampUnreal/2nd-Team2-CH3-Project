#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Pill.generated.h"

class AInfectedCityCharacter;

UCLASS()
class INFECTEDCITY_API APill : public AActor, public IItemBase
{
    GENERATED_BODY()

public:
    APill();

    virtual void UseItem_Implementation(AActor* User) override;

    void ResetStamina(AInfectedCityCharacter* Player);

private:
    FTimerHandle StaminaResetTimerHandle;  // ���¹̳� ���� Ÿ�̸� �ڵ�
};

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
    FTimerHandle StaminaResetTimerHandle;  // 스태미나 복원 타이머 핸들
};

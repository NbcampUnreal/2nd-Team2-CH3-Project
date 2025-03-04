#include "Bandage.h"
#include "InfectedCity/InfectedCityCharacter.h"

ABandage::ABandage()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABandage::UseItem_Implementation(AActor* User)
{
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (Player)
    {
        // 플레이어의 체력 10 회복 (예제: Player가 HP 변수를 가지고 있다고 가정)
        Player->CurrentHP = FMath::Clamp(Player->CurrentHP + 10, 0, Player->MaxHP);
    }
}

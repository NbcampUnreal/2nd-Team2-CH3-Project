#include "Bandage.h"
#include "./InfectedCity/InfectedCityCharacter.h"

ABandage::ABandage()
{
	PrimaryActorTick.bCanEverTick = true;

    ItemType = "Bandage";
}

void ABandage::UseItem(AActor* User)
{
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (Player)
    {
        float HealAmount = 20.0f;
        float CurrentHP = Player->GetCurrentHP();
        Player->UpdateHP(CurrentHP + HealAmount);
    }
}


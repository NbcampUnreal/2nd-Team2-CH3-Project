#include "Bandage.h"
#include "./InfectedCity/InfectedCityCharacter.h"

ABandage::ABandage()
{
	PrimaryActorTick.bCanEverTick = true;

    ItemType = "Bandage";
}

void ABandage::UseItem_Implementation(AActor* User)
{
    
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (Player)
    {
        // �÷��̾��� ü�� 10 ȸ�� (����: Player�� HP ������ ������ �ִٰ� ����)
        Player->CurrentHP = FMath::Clamp(Player->CurrentHP + 10, 0, Player->MaxHP);
    }

    
}

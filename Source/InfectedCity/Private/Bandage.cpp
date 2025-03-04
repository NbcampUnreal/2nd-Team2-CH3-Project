#include "Bandage.h"

ABandage::ABandage()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemType = "Bandage";
}

void ABandage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

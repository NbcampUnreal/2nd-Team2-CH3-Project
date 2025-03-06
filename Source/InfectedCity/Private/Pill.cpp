#include "Pill.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "HUDWidget.h"

APill::APill()
{
    PrimaryActorTick.bCanEverTick = false;

    ItemType = "Pill";
}

void APill::UseItem(AActor* User)
{
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("UseItem() - Player�� NULL�Դϴ�!"));
        return;
    }

    // ���¹̳��� 100���� ����
    Player->Stamina = 100.0f;

    // HUD ������Ʈ
    if (Player->HUDWidget)
    {
        Player->HUDWidget->UpdateStaminaBar(Player->Stamina / Player->MaxStamina);
    }

    UE_LOG(LogTemp, Warning, TEXT("UseItem() - ���¹̳��� 100���� ������!"));
}

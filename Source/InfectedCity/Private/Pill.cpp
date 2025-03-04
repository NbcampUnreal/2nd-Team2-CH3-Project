#include "Pill.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "TimerManager.h"
#include "HUDWidget.h"  // UHUDWidget�� �����մϴ�.

APill::APill()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APill::UseItem_Implementation(AActor* User)
{
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (Player)
    {
        // �÷��̾��� ���¹̳��� 100���� ����
        Player->Stamina = 100.0f;

        if (Player->HUDWidget)
        {
            Player->HUDWidget->UpdateStaminaBar(Player->Stamina / Player->MaxStamina);
        }

        GetWorld()->GetTimerManager().SetTimer(
            StaminaResetTimerHandle,
            FTimerDelegate::CreateUObject(this, &APill::ResetStamina, Player),
            5.0f,
            false
        );
    }
}

void APill::ResetStamina(AInfectedCityCharacter* Player)
{
    if (Player)
    {
        // ���� ���¹̳��� ����
        Player->Stamina = Player->MaxStamina;

        // HUD ������Ʈ
        if (Player->HUDWidget)
        {
            Player->HUDWidget->UpdateStaminaBar(Player->Stamina / Player->MaxStamina);
        }
    }
}

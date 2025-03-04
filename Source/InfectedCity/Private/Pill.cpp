#include "Pill.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "TimerManager.h"
#include "HUDWidget.h"  // UHUDWidget을 포함합니다.

APill::APill()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APill::UseItem_Implementation(AActor* User)
{
    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(User);
    if (Player)
    {
        // 플레이어의 스태미나를 100으로 설정
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
        // 원래 스태미나로 복원
        Player->Stamina = Player->MaxStamina;

        // HUD 업데이트
        if (Player->HUDWidget)
        {
            Player->HUDWidget->UpdateStaminaBar(Player->Stamina / Player->MaxStamina);
        }
    }
}

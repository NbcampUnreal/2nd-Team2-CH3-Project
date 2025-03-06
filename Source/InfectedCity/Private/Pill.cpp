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
        UE_LOG(LogTemp, Error, TEXT("UseItem() - Player가 NULL입니다!"));
        return;
    }

    // 스태미나를 100으로 설정
    Player->Stamina = 100.0f;

    // HUD 업데이트
    if (Player->HUDWidget)
    {
        Player->HUDWidget->UpdateStaminaBar(Player->Stamina / Player->MaxStamina);
    }

    UE_LOG(LogTemp, Warning, TEXT("UseItem() - 스태미나가 100으로 설정됨!"));
}

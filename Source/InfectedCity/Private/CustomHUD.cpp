#include "CustomHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"

void ACustomHUD::BeginPlay()
{
    Super::BeginPlay();

    // HUD ���� ���� �� ȭ�鿡 ǥ��
    if (HUDWidgetClass)  // HUDWidgetClass�� UI ������ Ŭ���� (��: UHUDWidget)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }
}
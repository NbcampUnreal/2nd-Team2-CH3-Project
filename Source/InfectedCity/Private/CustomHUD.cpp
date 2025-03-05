#include "CustomHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"

void ACustomHUD::BeginPlay()
{
    Super::BeginPlay();

    // HUD 위젯 생성 및 화면에 표시
    if (HUDWidgetClass)  // HUDWidgetClass는 UI 위젯의 클래스 (예: UHUDWidget)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }
}
#include "ClearMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AClearMenu::BeginPlay()
{
    Super::BeginPlay();
if (ClearWidgetClass)
{
    ClearWidget = CreateWidget<UUserWidget>(GetWorld(), ClearWidgetClass);
    if (ClearWidget)
    {
        ClearWidget->AddToViewport();

        // 마우스 커서 활성화 및 UI 입력 모드 설정
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;
            PC->SetInputMode(FInputModeUIOnly());
        }
    }
   }
}

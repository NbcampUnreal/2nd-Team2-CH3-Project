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

        // ���콺 Ŀ�� Ȱ��ȭ �� UI �Է� ��� ����
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;
            PC->SetInputMode(FInputModeUIOnly());
        }
    }
   }
}

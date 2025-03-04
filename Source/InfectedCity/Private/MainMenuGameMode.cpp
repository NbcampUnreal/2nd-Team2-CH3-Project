#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();


    if (BackgroundMusic)
    {
        AudioComponent = UGameplayStatics::SpawnSound2D(this, BackgroundMusic);
    }


    if (MenuWidgetClass)
    {
        MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();

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

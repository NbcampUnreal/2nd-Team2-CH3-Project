#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();


    if (BGM_Sound)
    {
        BGM_AudioComponent = UGameplayStatics::SpawnSound2D(this, BGM_Sound);
        if (BGM_AudioComponent)
        {
            BGM_AudioComponent->SetVolumeMultiplier(1.0f); // 초기 볼륨 설정 (1.0)
        }
    }

    if (MenuWidgetClass)
    {
        MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();

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

void AMainMenuGameMode::SetBGMVolume(float Volume)
{
    if (BGM_AudioComponent)
    {
        BGM_AudioComponent->SetVolumeMultiplier(Volume);
    }
}
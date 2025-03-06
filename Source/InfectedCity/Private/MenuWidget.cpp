#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "MainMenuGameMode.h"

bool UMenuWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    // 버튼이 정상적으로 바인딩되었는지 확인 후 이벤트 바인딩
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartClicked);
    }
    if (CreditButton)
    {
        CreditButton->OnClicked.AddDynamic(this, &UMenuWidget::OnCreditClicked);
    }
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitClicked);
    }

    if (VolumeSlider)
    {
        VolumeSlider->OnValueChanged.AddDynamic(this, &UMenuWidget::OnVolumeChanged);
    }

    return true;
}

void UMenuWidget::OnStartClicked()
{
    UGameplayStatics::OpenLevel(this, FName("LV_Soul_Slum")); // "GameLevel"은 게임의 실제 레벨 이름으로 변경해야 함
}

void UMenuWidget::OnCreditClicked()
{
    // 크레딧 이미지를 표시하는 로직 추가 (위젯의 가시성을 변경하는 방식)
    UE_LOG(LogTemp, Warning, TEXT("Credit Button Clicked!"));
}

void UMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMenuWidget::OnVolumeChanged(float Value)
{
    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
    AMainMenuGameMode* MainMenuGM = Cast<AMainMenuGameMode>(GameMode);

    if (MainMenuGM)
    {
        MainMenuGM->SetBGMVolume(Value);
    }
}
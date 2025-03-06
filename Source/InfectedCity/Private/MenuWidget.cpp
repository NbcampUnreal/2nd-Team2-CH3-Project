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

    // ��ư�� ���������� ���ε��Ǿ����� Ȯ�� �� �̺�Ʈ ���ε�
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
    UGameplayStatics::OpenLevel(this, FName("LV_Soul_Slum")); // "GameLevel"�� ������ ���� ���� �̸����� �����ؾ� ��
}

void UMenuWidget::OnCreditClicked()
{
    // ũ���� �̹����� ǥ���ϴ� ���� �߰� (������ ���ü��� �����ϴ� ���)
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
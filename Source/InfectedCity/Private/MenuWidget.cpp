#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

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
    UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap")); // "GameLevel"�� ������ ���� ���� �̸����� �����ؾ� ��
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
    // BGM_SoundClass�� ������ ����
    UGameplayStatics::SetSoundMixClassOverride(
        this,
        nullptr,                    // ���� �ͽ� (�⺻�� null)
        BGM_SoundClass,             // �츮�� ������ Sound Class
        Value,                      // �����̴����� ���� ���� ��
        1.0f,                       // ��ġ
        0.0f,                       // ���̵� �ð� (��� ����)
        true                        // ��� ����
    );
}
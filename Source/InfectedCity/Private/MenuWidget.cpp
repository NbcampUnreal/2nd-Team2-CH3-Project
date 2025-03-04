#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

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
    UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap")); // "GameLevel"은 게임의 실제 레벨 이름으로 변경해야 함
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
    // BGM_SoundClass의 볼륨을 조절
    UGameplayStatics::SetSoundMixClassOverride(
        this,
        nullptr,                    // 사운드 믹스 (기본값 null)
        BGM_SoundClass,             // 우리가 조절할 Sound Class
        Value,                      // 슬라이더에서 받은 볼륨 값
        1.0f,                       // 피치
        0.0f,                       // 페이드 시간 (즉시 변경)
        true                        // 즉시 적용
    );
}
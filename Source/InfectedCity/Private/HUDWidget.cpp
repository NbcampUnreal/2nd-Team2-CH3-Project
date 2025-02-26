#include "HUDWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CrouchImage) CrouchImage->SetVisibility(ESlateVisibility::Hidden);
    if (StandImage) StandImage->SetVisibility(ESlateVisibility::Visible);

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;

        PC->InputComponent->BindKey(EKeys::One, IE_Pressed, this, &UHUDWidget::OnKey1Pressed);
        PC->InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &UHUDWidget::OnKey2Pressed);
        PC->InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &UHUDWidget::OnKey3Pressed);
        PC->InputComponent->BindKey(EKeys::Four, IE_Pressed, this, &UHUDWidget::OnKey4Pressed);
        PC->InputComponent->BindKey(EKeys::Five, IE_Pressed, this, &UHUDWidget::OnKey5Pressed);
    }

    ImageMap.Add(1, MaingunImage);
    ImageMap.Add(2, SubgunImage);
    ImageMap.Add(3, MeleeImage);
    ProgressBarMap.Add(4, UseProgress);
    ProgressBarMap.Add(5, UseProgress);

    UseProgress->SetVisibility(ESlateVisibility::Hidden);

    if (AmmoProgressBar)
    {
        AmmoProgressBar->SetPercent(1.0f); // 시작 시 최대 탄약 상태로 설정
    }

}

void UHUDWidget::UpdateImageOpacity(int32 SelectedKey)
{
    for (auto& Elem : ImageMap)
    {
        float Opacity = (Elem.Key == SelectedKey) ? 1.0f : 0.1f;
        Elem.Value->SetRenderOpacity(Opacity);
    }
}

void UHUDWidget::OnKey1Pressed() { UpdateImageOpacity(1); }
void UHUDWidget::OnKey2Pressed() { UpdateImageOpacity(2); }
void UHUDWidget::OnKey3Pressed() { UpdateImageOpacity(3); }
void UHUDWidget::StartProgressBar(int32 Key, float Duration)
{
    if (bIsUsingProgress) return;  // 이미 진행 중이면 실행 안 함
    bIsUsingProgress = true;

    if (UProgressBar* ProgressBar = ProgressBarMap[Key])
    {
        ProgressBar->SetVisibility(ESlateVisibility::Visible);
        ProgressBar->SetPercent(0.0f);

        // 진행 시간 초기화
        ElapsedTime = 0.0f;

        // 0.1초마다 업데이트 (10번 = 1초)
        GetWorld()->GetTimerManager().SetTimer(ProgressBarUpdateHandle, [this, ProgressBar, Duration]()
            {
                ElapsedTime += 0.1f;  // 시간 증가
                float Percent = ElapsedTime / Duration;
                ProgressBar->SetPercent(Percent);

                if (Percent >= 1.0f)
                {
                    GetWorld()->GetTimerManager().ClearTimer(ProgressBarUpdateHandle);
                    ProgressBar->SetVisibility(ESlateVisibility::Hidden);
                    bIsUsingProgress = false;
                }
            }, 0.1f, true);
    }
}

void UHUDWidget::OnKey4Pressed() { StartProgressBar(4, 5.0f); }
void UHUDWidget::OnKey5Pressed() { StartProgressBar(5, 10.0f); }

void UHUDWidget::SetCrouchState(bool bIsCrouching)
{
    if (CrouchImage && StandImage)
    {
        CrouchImage->SetVisibility(bIsCrouching ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        StandImage->SetVisibility(bIsCrouching ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}

void UHUDWidget::UpdateAmmoUI(float AmmoPercentage)
{
    if (AmmoProgressBar)
    {
        AmmoProgressBar->SetPercent(AmmoPercentage);
        UE_LOG(LogTemp, Warning, TEXT("Ammo UI updated: %f"), AmmoPercentage);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AmmoProgressBar is nullptr!"));
    }
}


void UHUDWidget::DecreaseAmmo()
{
    if (CurrentAmmo > 0)
    {
        CurrentAmmo--;
        float AmmoPercentage = (float)CurrentAmmo / MaxAmmo;

        // UI 업데이트
        UpdateAmmoUI(AmmoPercentage);
    }
}

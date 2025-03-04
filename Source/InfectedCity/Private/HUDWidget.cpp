#include "HUDWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "Bandage.h"
#include "Pill.h"
#include "Components/InputComponent.h"

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

    if (BandageNum)
    {
        BandageNum->SetText(FText::FromString("0"));
    }

    if (StaminaProgressBar)
    {
        StaminaProgressBar->SetPercent(1.0f);
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

void UHUDWidget::OnKey4Pressed()
{
    AInfectedCityCharacter* Player = GetPlayerCharacter();
    if (!Player) return;

    int32* BandageCount = Player->Inventory.Find(ABandage::StaticClass());
    if (BandageCount && *BandageCount > 0)
    {
        StartProgressBar(4, 5.0f);
        Player->UseItem(ABandage::StaticClass());
        UpdateBandageCount(*BandageCount - 1);
    }
}

void UHUDWidget::OnKey5Pressed()
{
    AInfectedCityCharacter* Player = GetPlayerCharacter();
    if (!Player) return;

    int32* PillCount = Player->Inventory.Find(APill::StaticClass());
    if (PillCount && *PillCount > 0)
    {
        // 진행 바 시작
        StartProgressBar(5, 10.0f);

        // Pill 아이템 사용
        Player->UseItem(APill::StaticClass());

        // Pill 아이템 수 감소
        UpdatePillCount(*PillCount - 1);
    }
}

void UHUDWidget::SetCrouchState(bool bIsCrouching)
{
    if (CrouchImage && StandImage)
    {
        CrouchImage->SetVisibility(bIsCrouching ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        StandImage->SetVisibility(bIsCrouching ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}
void UHUDWidget::UpdateAmmoProgress(float NewAmmoValue)
{
    if (AmmoProgressBar)
    {
        AmmoProgressBar->SetPercent(NewAmmoValue);

        // Ammo가 0이면 Border 색상 변경 시작
        if (NewAmmoValue <= 0.0f)
        {
            UpdateBorderColor();
        }
    }
}

void UHUDWidget::UpdateBorderColor()
{
    if (Border)
    {
        BlinkCount = 0; // 깜빡임 횟수 초기화
        GetWorld()->GetTimerManager().SetTimer(BorderColorTimerHandle, this, &UHUDWidget::ToggleBorderColor, 0.25f, true);
    }
}

void UHUDWidget::ToggleBorderColor()
{
    if (Border)
    {
        bToggle = !bToggle; // 토글 상태 변경
        FLinearColor NewColor = bToggle ? Color2 : Color1;
        Border->SetBrushColor(NewColor);

        BlinkCount++;
        if (BlinkCount >= 10)
        {
            StopBorderColorBlink();
        }
    }
}

void UHUDWidget::StopBorderColorBlink()
{
    GetWorld()->GetTimerManager().ClearTimer(BorderColorTimerHandle);

    if (Border)
    {
        Border->SetBrushColor(Color1);
    }
}

void UHUDWidget::SetReloadTextVisibility(bool bIsVisible)
{
    if (ReloadText)
    {
        ReloadText->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UHUDWidget::PlayReloadAnimation()
{
    if (ReloadingText)
    {
        PlayAnimation(ReloadingText);
    }
}

AInfectedCityCharacter* UHUDWidget::GetPlayerCharacter()
{
    return Cast<AInfectedCityCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UHUDWidget::StartBandageIndicator()
{
    if (BandageIndicator)
    {
        BandageIndicator->SetVisibility(ESlateVisibility::Visible);
        GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &UHUDWidget::RotateBandageIndicator, 0.05f, true);
    }
}

void UHUDWidget::StopBandageIndicator()
{
    if (BandageIndicator)
    {
        BandageIndicator->SetVisibility(ESlateVisibility::Hidden);
    }
    GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
}

void UHUDWidget::RotateBandageIndicator()
{
    if (BandageIndicator)
    {
        RotationAngle += 5.0f;
        if (RotationAngle >= 360.0f)
        {
            RotationAngle = 0.0f;
        }
        FWidgetTransform NewTransform;
        NewTransform.Angle = RotationAngle;
        BandageIndicator->SetRenderTransform(NewTransform);
    }
}


void UHUDWidget::UpdateBandageCount(int32 Count)
{
    if (BandageNum)
    {
        BandageNum->SetText(FText::AsNumber(Count));
    }

    if (Count > 0)
    {
        StartBandageIndicator();
    }
    else
    {
        StopBandageIndicator();
    }
}

void UHUDWidget::UpdatePillCount(int32 Count)
{
    if (PillNum)
    {
        PillNum->SetText(FText::AsNumber(Count));
    }

    if (Count > 0)
    {
        StartPillIndicator();
    }
    else
    {
        StopPillIndicator();
    }
}

void UHUDWidget::UpdateStaminaBar(float StaminaRatio)
{
    if (StaminaProgressBar) 
    {
        StaminaProgressBar->SetPercent(StaminaRatio);
    }
}

void UHUDWidget::StartPillIndicator()
{
    if (PillIndicator)
    {
        PillIndicator->SetVisibility(ESlateVisibility::Visible);
        GetWorld()->GetTimerManager().SetTimer(PillRotationTimerHandle, this, &UHUDWidget::RotatePillIndicator, 0.05f, true);
    }
}

void UHUDWidget::StopPillIndicator()
{
    if (PillIndicator)
    {
        PillIndicator->SetVisibility(ESlateVisibility::Hidden);
    }
    GetWorld()->GetTimerManager().ClearTimer(PillRotationTimerHandle);
}

void UHUDWidget::RotatePillIndicator()
{
    if (PillIndicator)
    {
        PillRotationAngle += 5.0f;
        if (PillRotationAngle >= 360.0f)
        {
            PillRotationAngle = 0.0f;
        }
        FWidgetTransform NewTransform;
        NewTransform.Angle = PillRotationAngle;
        PillIndicator->SetRenderTransform(NewTransform);
    }
}
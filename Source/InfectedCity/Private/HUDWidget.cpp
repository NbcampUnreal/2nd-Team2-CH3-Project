#include "HUDWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
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
    if (bIsUsingProgress) return;  // �̹� ���� ���̸� ���� �� ��
    bIsUsingProgress = true;

    if (UProgressBar* ProgressBar = ProgressBarMap[Key])
    {
        ProgressBar->SetVisibility(ESlateVisibility::Visible);
        ProgressBar->SetPercent(0.0f);

        // ���� �ð� �ʱ�ȭ
        ElapsedTime = 0.0f;

        // 0.1�ʸ��� ������Ʈ (10�� = 1��)
        GetWorld()->GetTimerManager().SetTimer(ProgressBarUpdateHandle, [this, ProgressBar, Duration]()
            {
                ElapsedTime += 0.1f;  // �ð� ����
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
void UHUDWidget::UpdateAmmoProgress(float NewAmmoValue)
{
    if (AmmoProgressBar)
    {
        AmmoProgressBar->SetPercent(NewAmmoValue);

        // Ammo�� 0�̸� Border ���� ���� ����
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
        BlinkCount = 0; // ������ Ƚ�� �ʱ�ȭ
        GetWorld()->GetTimerManager().SetTimer(BorderColorTimerHandle, this, &UHUDWidget::ToggleBorderColor, 0.25f, true);
    }
}

void UHUDWidget::ToggleBorderColor()
{
    if (Border)
    {
        bToggle = !bToggle; // ��� ���� ����
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
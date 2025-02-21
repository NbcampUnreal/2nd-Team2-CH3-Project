#include "HUDWidget.h"
#include "Components/Image.h"

void UHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CrouchImage) CrouchImage->SetVisibility(ESlateVisibility::Hidden);
    if (StandImage) StandImage->SetVisibility(ESlateVisibility::Visible);

}

void UHUDWidget::SetCrouchState(bool bIsCrouching)
{
    if (CrouchImage && StandImage)
    {
        CrouchImage->SetVisibility(bIsCrouching ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        StandImage->SetVisibility(bIsCrouching ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}
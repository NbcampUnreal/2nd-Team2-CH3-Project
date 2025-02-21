#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

UCLASS()
class INFECTEDCITY_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()


public:
    virtual void NativeConstruct() override;

    /* Crouch 상태에 따라 UI 변경 */
        UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetCrouchState(bool bIsCrouching);

protected:
    /* Crouch 이미지 */
        UPROPERTY(meta = (BindWidget))
    class UImage* CrouchImage;

    /* Stand 이미지 */
    UPROPERTY(meta = (BindWidget))
    class UImage* StandImage;
};
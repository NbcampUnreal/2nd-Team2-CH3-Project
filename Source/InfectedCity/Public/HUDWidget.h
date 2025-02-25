#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
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

    UPROPERTY(meta = (BindWidget)) UImage* MaingunImage;
    UPROPERTY(meta = (BindWidget)) UImage* SubgunImage;
    UPROPERTY(meta = (BindWidget)) UImage* MeleeImage;
    UPROPERTY(meta = (BindWidget)) UProgressBar* UseProgress;
    UPROPERTY(meta = (BindWidget)) UImage* CrouchImage;
    UPROPERTY(meta = (BindWidget)) UImage* StandImage;

    void OnKey1Pressed();
    void OnKey2Pressed();
    void OnKey3Pressed();
    void OnKey4Pressed();
    void OnKey5Pressed();

    void UpdateImageOpacity(int32 SelectedKey);
    void StartProgressBar(int32 Key, float Duration);

    bool bIsUsingProgress = false;
    FTimerHandle ProgressBarTimerHandle;
    FTimerHandle ProgressBarUpdateHandle;

    TMap<int32, UImage*> ImageMap;
    TMap<int32, UProgressBar*> ProgressBarMap;

    float ElapsedTime = 0.0f;
};
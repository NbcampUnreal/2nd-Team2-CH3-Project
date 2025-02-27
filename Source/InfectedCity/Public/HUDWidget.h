#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class INFECTEDCITY_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()


public:
    virtual void NativeConstruct() override;

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
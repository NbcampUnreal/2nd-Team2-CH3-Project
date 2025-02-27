#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
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

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* ReloadText;

    // Reload Text ±ôºýÀÌ°Ô ÇÏ´Â ÇÔ¼ö
    void StartFlashingReloadText();
    void StopFlashingReloadText();
    void ToggleReloadTextVisibility();

    FTimerHandle ReloadTextFlashTimer;
    bool bIsReloadTextVisible = true;


    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetCrouchState(bool bIsCrouching);

    UPROPERTY(meta = (BindWidget)) UImage* MaingunImage;
    UPROPERTY(meta = (BindWidget)) UImage* SubgunImage;
    UPROPERTY(meta = (BindWidget)) UImage* MeleeImage;
    UPROPERTY(meta = (BindWidget)) UProgressBar* UseProgress;
    UPROPERTY(meta = (BindWidget)) UImage* CrouchImage;
    UPROPERTY(meta = (BindWidget)) UImage* StandImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UProgressBar* AmmoProgressBar;

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void UpdateAmmoProgress(float NewAmmoValue);


    UPROPERTY(BlueprintReadWrite, Category = "Ammo")
    int32 CurrentAmmo = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Ammo")
    int32 MaxAmmo;

    void OnKey1Pressed();
    void OnKey2Pressed();
    void OnKey3Pressed();
    void OnKey4Pressed();
    void OnKey5Pressed();

    void UpdateImageOpacity(int32 SelectedKey);

    void StartProgressBar(int32 Key, float Duration);

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY()
    UHUDWidget* HUDWidget;

    bool bIsUsingProgress = false;
    FTimerHandle ProgressBarTimerHandle;
    FTimerHandle ProgressBarUpdateHandle;

    TMap<int32, UImage*> ImageMap;
    TMap<int32, UProgressBar*> ProgressBarMap;

    float ElapsedTime = 0.0f;

};
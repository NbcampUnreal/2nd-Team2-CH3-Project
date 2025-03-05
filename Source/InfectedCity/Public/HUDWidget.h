#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UImage;
class UProgressBar;
class TextBlock;
class AInfectedCityCharacter;

UCLASS()
class INFECTEDCITY_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FLinearColor Color1 = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FLinearColor Color2 = FLinearColor::Red;

    UPROPERTY(meta = (BindWidget))
    UBorder* Border;

    UFUNCTION(BlueprintCallable)
    void UpdateBorderColor();

    void ToggleBorderColor();

    UFUNCTION(BlueprintCallable)
    void StopBorderColorBlink();

    UFUNCTION(BlueprintCallable)
    void SetReloadTextVisibility(bool bIsVisible);

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ReloadText;

    UFUNCTION(BlueprintCallable)
    void PlayReloadAnimation();

    UPROPERTY(meta = (BindWidget))
    UTextBlock* BandageNum;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PillNum;

    UFUNCTION(BlueprintCallable)
    void UpdateBandageCount(int32 Count);

    UFUNCTION(BlueprintCallable)
    void UpdatePillCount(int32 Count);

    UFUNCTION(BlueprintCallable)
    void UpdateStaminaBar(float StaminaRatio);

    UFUNCTION(BlueprintCallable)
    void StartPillIndicator();

    UFUNCTION(BlueprintCallable)
    void StopPillIndicator();

    UFUNCTION(BlueprintCallable)
    void RotatePillIndicator();

    UPROPERTY(meta = (BindWidget))
    UImage* PillIndicator;

    FTimerHandle PillRotationTimerHandle;
    float PillRotationAngle = 0.0f;

    void UpdateHPBar(float HPRatio);

    UPROPERTY(meta = (BindWidget))
    UImage* GasCanImg;

    void UpdateGasCount(int32 Count);

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Story01Text;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Story02Text;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Story03Text;

    FTimerHandle StoryTimerHandle;
    FTimerHandle CountdownTimerHandle;

    void StartRideSequence();  // canride가 true가 되었을 때 호출
    void HideStory01AndStartCountdown(); // story01을 숨기고 카운트다운 시작
    void UpdateCountdown();  // story02의 텍스트 업데이트

    bool Canout = false;

protected:
    FTimerHandle BorderColorTimerHandle;

    int32 BlinkCount = 0;

    bool bToggle = false;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* ReloadingText;

    UPROPERTY(meta = (BindWidget))
        class UProgressBar* StaminaProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPProgressBar;

    int32 CountdownTime = 180;

private:
    AInfectedCityCharacter* GetPlayerCharacter();

    UPROPERTY(meta = (BindWidget))
    UImage* BandageIndicator;

    FTimerHandle RotationTimerHandle;
    float RotationAngle = 0.0f;

    void StartBandageIndicator();
    void StopBandageIndicator();
    void RotateBandageIndicator();
};
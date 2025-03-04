#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "MenuWidget.generated.h"

UCLASS()
class INFECTEDCITY_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    class USoundClass* BGM_SoundClass;

protected:
    virtual bool Initialize() override;

private:
    UFUNCTION()
    void OnStartClicked();

    UFUNCTION()
    void OnCreditClicked();

    UFUNCTION()
    void OnExitClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CreditButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

    UFUNCTION()
    void OnVolumeChanged(float Value);

    UPROPERTY(meta = (BindWidget))
    class USlider* VolumeSlider;
};

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

    /* Crouch ���¿� ���� UI ���� */
        UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetCrouchState(bool bIsCrouching);

protected:
    /* Crouch �̹��� */
        UPROPERTY(meta = (BindWidget))
    class UImage* CrouchImage;

    /* Stand �̹��� */
    UPROPERTY(meta = (BindWidget))
    class UImage* StandImage;
};
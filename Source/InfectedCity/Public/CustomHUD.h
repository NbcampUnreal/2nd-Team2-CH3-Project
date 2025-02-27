#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDWidget.h"
#include "CustomHUD.generated.h"

UCLASS()
class INFECTEDCITY_API ACustomHUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    class UHUDWidget* HUDWidget;

    // 위젯을 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "HUD")
    UHUDWidget* GetHUDWidget() const { return HUDWidget; }

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "HUD")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

};

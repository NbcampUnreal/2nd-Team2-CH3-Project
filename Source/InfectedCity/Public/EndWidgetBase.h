#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndWidgetBase.generated.h"

UCLASS()
class INFECTEDCITY_API UEndWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    // 게임 처음부터 시작하는 함수
    UFUNCTION()
    void OnGoMainClicked();

    // 게임 종료 함수
    UFUNCTION()
    void OnGoWindowClicked();

protected:
    virtual bool Initialize() override;

    // 버튼을 위한 UPROPERTY 선언
    UPROPERTY(meta = (BindWidget))
    class UButton* GoMainButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GoWindowButton;
};

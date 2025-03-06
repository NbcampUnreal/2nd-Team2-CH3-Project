#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndWidgetBase.generated.h"

UCLASS()
class INFECTEDCITY_API UEndWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    // ���� ó������ �����ϴ� �Լ�
    UFUNCTION()
    void OnGoMainClicked();

    // ���� ���� �Լ�
    UFUNCTION()
    void OnGoWindowClicked();

protected:
    virtual bool Initialize() override;

    // ��ư�� ���� UPROPERTY ����
    UPROPERTY(meta = (BindWidget))
    class UButton* GoMainButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GoWindowButton;
};

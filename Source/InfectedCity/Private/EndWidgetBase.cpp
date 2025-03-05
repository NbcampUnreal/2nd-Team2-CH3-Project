#include "EndWidgetBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UEndWidgetBase::Initialize()
{
    Super::Initialize();

    if (GoMainButton)
    {
        GoMainButton->OnClicked.AddDynamic(this, &UEndWidgetBase::OnGoMainClicked);
    }
    if (GoWindowButton)
    {
        GoWindowButton->OnClicked.AddDynamic(this, &UEndWidgetBase::OnGoWindowClicked);
    }

    return true;
}

// ���� �޴��� �̵��ϴ� �Լ�
void UEndWidgetBase::OnGoMainClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Mainmenu")); // "Mainmenu" ������ �̵�
}

// ���� ���� �Լ�
void UEndWidgetBase::OnGoWindowClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

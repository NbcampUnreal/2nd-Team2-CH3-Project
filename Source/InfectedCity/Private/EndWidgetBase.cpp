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

// 메인 메뉴로 이동하는 함수
void UEndWidgetBase::OnGoMainClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Mainmenu")); // "Mainmenu" 레벨로 이동
}

// 게임 종료 함수
void UEndWidgetBase::OnGoWindowClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

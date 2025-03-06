#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClearMenu.generated.h"

UCLASS()
class INFECTEDCITY_API AClearMenu : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> ClearWidgetClass;

private:
    UUserWidget* ClearWidget;

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class INFECTEDCITY_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    class USoundBase* BackgroundMusic;

private:
    UUserWidget* MenuWidget;

    UPROPERTY()
    class UAudioComponent* AudioComponent;
};

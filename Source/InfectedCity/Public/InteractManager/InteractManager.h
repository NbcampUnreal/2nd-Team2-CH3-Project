#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractManager.generated.h"

UCLASS()
class INFECTEDCITY_API UInteractManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void AddInteractActor(FName Key, AActor* NewActor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void RemoveInteractActor(FName Key);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractWith(FName Key);

private:
    TMultiMap<FName, AActor*> InteractableActors;
};

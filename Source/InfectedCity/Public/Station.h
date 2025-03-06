#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Station.generated.h"

class USceneComponent;
class UBoxComponent;
class UTextRenderComponent;
class AInfectedCityCharacter;

UCLASS()
class INFECTEDCITY_API AStation : public AActor
{
    GENERATED_BODY()

public:
    AStation();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
    int32 StationCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
    bool CanOut = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
    bool Drinkgas = false;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UTextRenderComponent* TextComponent;

    AInfectedCityCharacter* OverlappingPawn;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void HandleInput();
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossObserverInterface.h"
#include "BossPattern.h"
#include "Boss.generated.h"

class UStaticMeshComponent;
class UAnimationAsset;

UCLASS()
class INFECTEDCITY_API ABoss : public AActor
{
    GENERATED_BODY()

public:
    ABoss();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Boss")
    USkeletalMeshComponent* SkeletalMeshComponent;

    UPROPERTY(EditAnywhere, Category = "Boss")
    UAnimationAsset* IdleAnimationAsset;

    UPROPERTY(EditAnywhere, Category = "Boss")
    TSubclassOf<class ABossSphere> BossSphereClass;

    UPROPERTY()
    TArray<TScriptInterface<IBossObserverInterface>> Observers;

    UPROPERTY(EditAnywhere, Category = "Boss")
    int32 NumSpheres{ 5 };

    UPROPERTY(EditAnywhere, Category = "Boss")
    FVector LocationOffset;

    EBossPattern CurrentPattern{ EBossPattern::Idle };

    void RegisterObserver(TScriptInterface<IBossObserverInterface> Observer);
    void UnregisterObserver(TScriptInterface<IBossObserverInterface> Observer);
    void NotifyObservers();
    void SpawnSpheres();
    void ChangePattern();

    FVector GetActorLocationToOffset() const { return GetActorLocation() + LocationOffset; }

public:
    UPROPERTY(EditAnywhere, Category = "Boss|Sphere")
    float OribitSpeed{ 100 };

    UPROPERTY(EditAnywhere, Category = "Boss|Sphere")
    float OribitRadius{ 200 };

    FTransform Transform;

    FVector RandomRotationAxis;
    float AxisChangeTime;
    float CurrentAxisTime;
    
private:
    void IdleAnimation(float DeltaTime);
};

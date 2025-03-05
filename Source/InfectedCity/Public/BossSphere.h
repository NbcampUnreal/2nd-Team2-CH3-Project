#pragma once

#include "CoreMinimal.h"
#include "BossPattern.h"
#include "GameFramework/Actor.h"
#include "BossObserverInterface.h"
#include "BossSphere.generated.h"

UCLASS()
class INFECTEDCITY_API ABossSphere : public AActor, public IBossObserverInterface
{
    GENERATED_BODY()

public:
    ABossSphere();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Boss")
    class UStaticMeshComponent* MeshComponent;

    void OnPatternChanged(EBossPattern NewPattern) override;

    void SetBoss(ABoss* InBoss);

private:
    class ABoss* Boss{ nullptr };
    float OrbitSpeed = 50.0f;
    float CurrentAngle = 0.0f;
};

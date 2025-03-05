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

    void SetBoss(class ABoss* InitialBoss, int32 InitialUniqueId, float InitialAngle, float* InitialOrbitSpeed, float* InitialOrbitRadius);

    void IdleAnimation(float DeltaTime);
private:
    class ABoss*    Boss{ nullptr };

    int32           UniquId{ 0 };

    float           CurrentAngle{ 0.0f };
    float*          OrbitSpeed{ nullptr };
    float*          OrbitRadius{ nullptr };

    FTransform      LocalTransform;
    FTransform      TestTransform;
};

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

    UPROPERTY(EditAnywhere, Category = "BossSphere")
    class UStaticMeshComponent* MeshComponent;

    UMaterialInstanceDynamic* DynMaterialInstance;

    UPROPERTY(EditAnywhere, Category = "BossSphere")
    class USphereComponent* SphereCollider;


    void OnPatternChanged(EBossPattern NewPattern) override;

    void SetBoss(class ABoss* InitialBoss, int32 InitialUniqueId, float InitialAngle, float* InitialOrbitSpeed, float* InitialOrbitRadius);

    void IdleAnimation(float DeltaTime);

    void EnableTick();

    void CustomCollisionCheck();
private:
    class ABoss*    Boss{ nullptr };

    int32           UniqueId{ 0 };

    float           CurrentAngle{ 0.0f };
    float*          OrbitSpeed{ nullptr };
    float*          OrbitRadius{ nullptr };

    FTransform      LocalTransform;
    FTransform      IdleWorldTransform;
    FTimerHandle    EnableTimerHandle;

private: /* Parttern Member */

    float           WarningAccTime{ 0.f };
    float           WarningDuration{ 0.f };

    float           PatternAccTime{ 0.f };
    float           PatternDuration{ 0.f };

    bool            bWarningFinished{ false };
    bool            bPatternFinished{ false };

    bool            bFinishedLock{ false };
    FVector         OriginalLocation{};

    void HeavyCrash(float DeltaTime);

private: /* Move Member */

    bool            bMoveToChange{ false };
    float           AccMoveTime{ 0.f };
    float           MoveInSecond{ 0.f };
    FVector         TargetPoint;
    FVector         MoveStartPoint;

    void TargetToMove(float DeltaTime);
    void InitializeAfterMoveDone();
    
};

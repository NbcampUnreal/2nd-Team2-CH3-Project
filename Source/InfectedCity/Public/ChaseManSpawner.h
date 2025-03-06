#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GN_ChaseMan.h"
#include "ChaseManSpawner.generated.h"

UCLASS()
class INFECTEDCITY_API AChaseManSpawner : public AActor
{
    GENERATED_BODY()

public:
    AChaseManSpawner();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnMobs();

private:
    /** 씬 루트 */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USceneComponent* Root;

    /** 스폰 범위를 정의하는 박스 콜라이더 */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* SpawnArea;

    /** 스폰할 액터 클래스 (블루프린트에서 설정 가능) */
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AGN_ChaseMan> MobClass;

    /** 스폰할 개수 */
    UPROPERTY(EditAnywhere, Category = "Spawning")
    int32 SpawnCount = 0;
};

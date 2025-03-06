#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobSpawner.generated.h"

UCLASS()
class INFECTEDCITY_API AMobSpawner : public AActor
{
    GENERATED_BODY()

public:
    AMobSpawner();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnMobs();

private:
    /** �� ��Ʈ */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USceneComponent* Root;

    /** ���� ������ �����ϴ� �ڽ� �ݶ��̴� */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* SpawnArea;

    /** ������ ���� Ŭ���� (�������Ʈ���� ���� ����) */
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MobClass;

    /** ������ ���� */
    UPROPERTY(EditAnywhere, Category = "Spawning")
    int32 SpawnCount = 0;
};

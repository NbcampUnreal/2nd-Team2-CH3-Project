#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"  

UCLASS()
class INFECTEDCITY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
    // ������
    ABullet();

    // �Ѿ��� ����� �ӵ� ����
    void InitializeBullet(FVector Direction, float Speed);

    // �Ѿ��� �浹���� �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
    // �Ѿ��� �浹 ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Bullet")
    class USphereComponent* CollisionComponent;

    // �Ѿ��� �޽� ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Bullet")
    class UStaticMeshComponent* MeshComponent;

    // �Ѿ��� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Bullet")
    float Speed;

    // �Ѿ��� ������
    UPROPERTY(EditAnywhere, Category = "Bullet")
    float Damage;

    // �浹 �� ������ ����Ʈ
    UPROPERTY(EditAnywhere, Category = "Bullet")
    class UParticleSystem* HitEffect;

    // �Ѿ��� �̵� ����
    FVector Direction;

    // �Ѿ� �̵� �Լ�
    void MoveBullet();
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"  
#include "Bullet.generated.h"  

UCLASS()
class INFECTEDCITY_API ABullet : public AActor
{
    GENERATED_BODY()


public:
    // ������
    ABullet();




public:
    // Tick �Լ�, �� �����Ӹ��� ȣ���
    virtual void Tick(float DeltaTime) override;


    // �Ѿ��� �̵� ����� �ӵ� ����
    void InitializeBullet(FVector InDirection, float InSpeed);

    // �Ѿ� �̵� �Լ�
    void MoveBullet(float DeltaTime);

    // �Ѿ��� �浹�� �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    // �Ѿ� �߻� �Լ�
    void Fire(FVector Direction, float Speed);
    void HandleBulletLifeTime(float DeltaTime);
private:
    float BulletSpeed = 1000000.f;  // �Ѿ� �ӵ�
    float Speed;
    float Damage;
    float BulletLifeTime = 0.f;  // �Ѿ� ���� �ð�
    float MaxLifeTime = 5.f;     // �ִ� ���� �ð� (5�� ����)
    bool bIsDestroyed = false;   // �Ѿ��� �ı��Ǿ����� ����
    bool bIsHit = false;

    // �Ѿ� �̵� ����
    FVector Direction;
    UPROPERTY(EditDefaultsOnly, Category = "Bullet")
    UMaterialInterface* DecalMaterial;
    // �浹 ������Ʈ
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    // �Ѿ� �޽� ������Ʈ
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* HitEffect;

};
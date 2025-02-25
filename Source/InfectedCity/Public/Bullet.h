#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"  

UCLASS()
class INFECTEDCITY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
    ABullet();

    // �⺻ �Լ�
    virtual void Tick(float DeltaTime) override;

    // �Ѿ� �ʱ�ȭ �Լ�
    void InitializeBullet(FVector InDirection, float InSpeed);

    // �浹 ó�� �Լ�
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
    // �浹 ������Ʈ �� �޽� ����
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    // �Ѿ� �̵� ����
    FVector Direction;

    // �Ѿ� �̵� �ӵ�
    float Speed;

    // �浹 ����Ʈ
    UPROPERTY(EditAnywhere)
    class UParticleSystem* HitEffect;

    // ������
    float Damage;

    // �Ѿ� �̵� ó��
    void MoveBullet();
};
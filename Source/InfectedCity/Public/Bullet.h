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

    // 기본 함수
    virtual void Tick(float DeltaTime) override;

    // 총알 초기화 함수
    void InitializeBullet(FVector InDirection, float InSpeed);

    // 충돌 처리 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
    // 충돌 컴포넌트 및 메시 설정
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    // 총알 이동 방향
    FVector Direction;

    // 총알 이동 속도
    float Speed;

    // 충돌 이펙트
    UPROPERTY(EditAnywhere)
    class UParticleSystem* HitEffect;

    // 데미지
    float Damage;

    // 총알 이동 처리
    void MoveBullet();
};
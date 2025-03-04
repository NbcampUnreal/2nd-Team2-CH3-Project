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
    // 생성자
    ABullet();




public:
    // Tick 함수, 매 프레임마다 호출됨
    virtual void Tick(float DeltaTime) override;


    // 총알의 이동 방향과 속도 설정
    void InitializeBullet(FVector InDirection, float InSpeed);

    // 총알 이동 함수
    void MoveBullet(float DeltaTime);

    // 총알이 충돌할 때 호출되는 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    // 총알 발사 함수
    void Fire(FVector Direction, float Speed);
    void HandleBulletLifeTime(float DeltaTime);
private:
    float BulletSpeed = 1000000.f;  // 총알 속도
    float Speed;
    float Damage;
    float BulletLifeTime = 0.f;  // 총알 생명 시간
    float MaxLifeTime = 5.f;     // 최대 생명 시간 (5초 예시)
    bool bIsDestroyed = false;   // 총알이 파괴되었는지 여부
    bool bIsHit = false;

    // 총알 이동 방향
    FVector Direction;
    UPROPERTY(EditDefaultsOnly, Category = "Bullet")
    UMaterialInterface* DecalMaterial;
    // 충돌 컴포넌트
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    // 총알 메시 컴포넌트
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* HitEffect;

};
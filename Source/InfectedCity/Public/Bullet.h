#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"  

UCLASS()
class INFECTEDCITY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
    // 생성자
    ABullet();

    // 총알의 방향과 속도 설정
    void InitializeBullet(FVector Direction, float Speed);

    // 총알이 충돌했을 때 호출되는 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
    // 총알의 충돌 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Bullet")
    class USphereComponent* CollisionComponent;

    // 총알의 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Bullet")
    class UStaticMeshComponent* MeshComponent;

    // 총알의 속도
    UPROPERTY(EditAnywhere, Category = "Bullet")
    float Speed;

    // 총알의 데미지
    UPROPERTY(EditAnywhere, Category = "Bullet")
    float Damage;

    // 충돌 시 생성될 이펙트
    UPROPERTY(EditAnywhere, Category = "Bullet")
    class UParticleSystem* HitEffect;

    // 총알의 이동 방향
    FVector Direction;

    // 총알 이동 함수
    void MoveBullet();
};
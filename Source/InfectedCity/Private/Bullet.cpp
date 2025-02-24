#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// 생성자
ABullet::ABullet()
{
    // 충돌 컴포넌트 및 메시 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Bullet"));
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    Speed = 1000.f;
    Damage = 10.f;

    // 충돌 시 이벤트 등록
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// 총알의 방향과 속도 설정
void ABullet::InitializeBullet(FVector InDirection, float InSpeed)
{
    Direction = InDirection.GetSafeNormal();
    Speed = InSpeed;

    // 총알 이동 시작
    MoveBullet();
}

// 총알 이동 처리
void ABullet::MoveBullet()
{
    FVector NewLocation = GetActorLocation() + (Direction * Speed * GetWorld()->GetDeltaSeconds());
    SetActorLocation(NewLocation);
}

// 충돌 처리
void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // 충돌 시 이펙트 처리
    if (HitEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator::ZeroRotator, true);
    }

    // 충돌한 액터에게 데미지 적용 (예: 적에게 데미지)
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
        // 데미지 처리 로직 (예: 데미지 함수 호출)
    }

    // 충돌 후 삭제
    Destroy();
}
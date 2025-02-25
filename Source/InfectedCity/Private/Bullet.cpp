#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;  // Tick 활성화

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

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveBullet();  // 매 프레임마다 총알 이동
}

void ABullet::InitializeBullet(FVector InDirection, float InSpeed)
{
    Direction = InDirection.GetSafeNormal();
    Speed = InSpeed;
}

void ABullet::MoveBullet()
{
    FVector NewLocation = GetActorLocation() + (Direction * Speed * GetWorld()->GetDeltaSeconds());
    SetActorLocation(NewLocation);
}

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
      
    }

    // 충돌 후 삭제
    Destroy();
}
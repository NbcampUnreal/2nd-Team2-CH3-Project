#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;  // Tick Ȱ��ȭ

    // �浹 ������Ʈ �� �޽� ����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Bullet"));
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    Speed = 1000.f;
    Damage = 10.f;

    // �浹 �� �̺�Ʈ ���
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveBullet();  // �� �����Ӹ��� �Ѿ� �̵�
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
    // �浹 �� ����Ʈ ó��
    if (HitEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator::ZeroRotator, true);
    }

    // �浹�� ���Ϳ��� ������ ���� (��: ������ ������)
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
      
    }

    // �浹 �� ����
    Destroy();
}
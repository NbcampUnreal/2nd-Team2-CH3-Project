#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"  // 디버깅용
#include <GN_Character.h>

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;  // Tick 활성화

    // 충돌 컴포넌트 및 메시 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.f);

    // 충돌 프로파일을 BlockAll로 설정하여 모든 물체와 충돌하도록
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 충돌과 물리 계산 모두 활성화
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);  // 모든 채널에 대해 블록 응답 설정
    CollisionComponent->SetSimulatePhysics(true);  // 물리 엔진 활성화

    RootComponent = CollisionComponent;
    CollisionComponent->bVisualizeComponent = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);


    Damage = 10.f;

    // 충돌 시 이벤트 등록
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

void ABullet::Tick(float DeltaTime)
{
    if (!bIsHit)  // 총알이 아직 충돌하지 않았으면 이동
    {
        MoveBullet(DeltaTime);  // 매 프레임마다 총알 이동
    }

    HandleBulletLifeTime(DeltaTime);  // 총알의 생명 주기 관리
}

void ABullet::InitializeBullet(FVector InDirection, float InSpeed)
{
    Direction = InDirection.GetSafeNormal();  // 총알의 이동 방향 설정
    Speed = InSpeed;  // 속도 설정
}

void ABullet::MoveBullet(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();  // 현재 위치
    FVector NewLocation = CurrentLocation + (Direction * Speed * DeltaTime);  // 새로운 위치

    // Sweep을 사용하여 충돌을 감지하면서 이동
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);  // 총알 자신은 충돌 검사에서 제외

    // Sweep을 사용하여 충돌을 검사하고 이동
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        CurrentLocation,
        NewLocation,
        FQuat::Identity,  // 회전값 (회전이 없다면 FQuat::Identity)
        ECC_Visibility,   // 충돌 채널
        FCollisionShape::MakeSphere(CollisionComponent->GetScaledSphereRadius()),  // 충돌 형태: 구체
        CollisionParams
    );

    if (bHit)  // 충돌이 발생했다면
    {
        OnHit(HitResult.Component.Get(), HitResult.GetActor(), HitResult.Component.Get(), FVector::ZeroVector, HitResult);
    }
    else
    {
        SetActorLocation(NewLocation);  // 충돌이 없으면 새로운 위치로 설정
    }

    if (!bHit)
    {
        bool bEnemyHit = GetWorld()->SweepSingleByChannel(
            HitResult,
            CurrentLocation,
            NewLocation,
            FQuat::Identity,  // 회전값 (회전이 없다면 FQuat::Identity)
            ECC_GameTraceChannel1,   // 충돌 채널
            FCollisionShape::MakeSphere(CollisionComponent->GetScaledSphereRadius()),  // 충돌 형태: 구체
            CollisionParams
        );
        
        if (bEnemyHit)
        {
            AGN_Character* Enemy = Cast<AGN_Character>(HitResult.GetActor());
            
            if (Enemy)
            {

                Enemy->CurrentHealth -= 100;

                if (Enemy->CurrentHealth <= 0)
                {
                    Enemy->Die();
                }
            }
        }
    }

}
void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // 충돌이 발생했는지 로그로 확인
    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());

    // 총알이 자신과 충돌하지 않도록 처리
    if (OtherActor && OtherActor != this)  // 자신과의 충돌을 제외
    {
        // 데칼 생성 (충돌한 위치와 표면에 데칼 추가)
        if (DecalMaterial)  // 데칼 머티리얼이 설정되어 있다면
        {
            UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                DecalMaterial,  // 사용할 데칼 머티리얼
                FVector(20.f, 10.f, 1.f),  // 데칼 크기
                Hit.ImpactPoint,  // 충돌 지점
                Hit.ImpactNormal.Rotation(),  // 충돌 표면의 회전값
                5.f  // 데칼의 지속 시간 (초 단위)
            );
        }
        if (HitEffect)  // 파티클 시스템이 설정되어 있다면
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                HitEffect,  // 사용할 파티클 시스템
                Hit.ImpactPoint,  // 파티클의 시작 위치 (충돌 지점)
                Hit.ImpactNormal.Rotation()  // 파티클의 회전값 (충돌 표면 방향)
            );
        }
        // 데미지 처리
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

        // 디버깅: 충돌 지점에 빨간색 구체 표시
        //DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f); 


        bIsHit = true;  // 충돌이 일어난 후, 더 이상 이동하지 않도록 플래그 설정

        // 총알 파괴
        Destroy();
    }
}

// 총알 발사 함수
void ABullet::Fire(FVector InDirection, float InSpeed)
{
    InitializeBullet(InDirection, InSpeed);  // 총알의 속도와 방향을 초기화
    FVector StartLocation = GetActorLocation();
    SetActorLocation(StartLocation);  // 발사 위치 설정
    bIsHit = false;  // 총알을 발사할 때는 충돌이 발생하지 않았음을 나타내는 플래그 설정
}

void ABullet::HandleBulletLifeTime(float DeltaTime)
{
    BulletLifeTime += DeltaTime;

    if (BulletLifeTime > MaxLifeTime)
    {
        Destroy();  // 총알이 너무 오래 살아있으면 파괴
    }
}
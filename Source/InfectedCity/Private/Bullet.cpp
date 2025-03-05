#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"  // ������
#include <GN_Character.h>

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;  // Tick Ȱ��ȭ

    // �浹 ������Ʈ �� �޽� ����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.f);

    // �浹 ���������� BlockAll�� �����Ͽ� ��� ��ü�� �浹�ϵ���
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // �浹�� ���� ��� ��� Ȱ��ȭ
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);  // ��� ä�ο� ���� ��� ���� ����
    CollisionComponent->SetSimulatePhysics(true);  // ���� ���� Ȱ��ȭ

    RootComponent = CollisionComponent;
    CollisionComponent->bVisualizeComponent = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);


    Damage = 10.f;

    // �浹 �� �̺�Ʈ ���
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

void ABullet::Tick(float DeltaTime)
{
    if (!bIsHit)  // �Ѿ��� ���� �浹���� �ʾ����� �̵�
    {
        MoveBullet(DeltaTime);  // �� �����Ӹ��� �Ѿ� �̵�
    }

    HandleBulletLifeTime(DeltaTime);  // �Ѿ��� ���� �ֱ� ����
}

void ABullet::InitializeBullet(FVector InDirection, float InSpeed)
{
    Direction = InDirection.GetSafeNormal();  // �Ѿ��� �̵� ���� ����
    Speed = InSpeed;  // �ӵ� ����
}

void ABullet::MoveBullet(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();  // ���� ��ġ
    FVector NewLocation = CurrentLocation + (Direction * Speed * DeltaTime);  // ���ο� ��ġ

    // Sweep�� ����Ͽ� �浹�� �����ϸ鼭 �̵�
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);  // �Ѿ� �ڽ��� �浹 �˻翡�� ����

    // Sweep�� ����Ͽ� �浹�� �˻��ϰ� �̵�
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        CurrentLocation,
        NewLocation,
        FQuat::Identity,  // ȸ���� (ȸ���� ���ٸ� FQuat::Identity)
        ECC_Visibility,   // �浹 ä��
        FCollisionShape::MakeSphere(CollisionComponent->GetScaledSphereRadius()),  // �浹 ����: ��ü
        CollisionParams
    );

    if (bHit)  // �浹�� �߻��ߴٸ�
    {
        OnHit(HitResult.Component.Get(), HitResult.GetActor(), HitResult.Component.Get(), FVector::ZeroVector, HitResult);
    }
    else
    {
        SetActorLocation(NewLocation);  // �浹�� ������ ���ο� ��ġ�� ����
    }

    if (!bHit)
    {
        bool bEnemyHit = GetWorld()->SweepSingleByChannel(
            HitResult,
            CurrentLocation,
            NewLocation,
            FQuat::Identity,  // ȸ���� (ȸ���� ���ٸ� FQuat::Identity)
            ECC_GameTraceChannel1,   // �浹 ä��
            FCollisionShape::MakeSphere(CollisionComponent->GetScaledSphereRadius()),  // �浹 ����: ��ü
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
    // �浹�� �߻��ߴ��� �α׷� Ȯ��
    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());

    // �Ѿ��� �ڽŰ� �浹���� �ʵ��� ó��
    if (OtherActor && OtherActor != this)  // �ڽŰ��� �浹�� ����
    {
        // ��Į ���� (�浹�� ��ġ�� ǥ�鿡 ��Į �߰�)
        if (DecalMaterial)  // ��Į ��Ƽ������ �����Ǿ� �ִٸ�
        {
            UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                DecalMaterial,  // ����� ��Į ��Ƽ����
                FVector(20.f, 10.f, 1.f),  // ��Į ũ��
                Hit.ImpactPoint,  // �浹 ����
                Hit.ImpactNormal.Rotation(),  // �浹 ǥ���� ȸ����
                5.f  // ��Į�� ���� �ð� (�� ����)
            );
        }
        if (HitEffect)  // ��ƼŬ �ý����� �����Ǿ� �ִٸ�
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                HitEffect,  // ����� ��ƼŬ �ý���
                Hit.ImpactPoint,  // ��ƼŬ�� ���� ��ġ (�浹 ����)
                Hit.ImpactNormal.Rotation()  // ��ƼŬ�� ȸ���� (�浹 ǥ�� ����)
            );
        }
        // ������ ó��
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

        // �����: �浹 ������ ������ ��ü ǥ��
        //DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f); 


        bIsHit = true;  // �浹�� �Ͼ ��, �� �̻� �̵����� �ʵ��� �÷��� ����

        // �Ѿ� �ı�
        Destroy();
    }
}

// �Ѿ� �߻� �Լ�
void ABullet::Fire(FVector InDirection, float InSpeed)
{
    InitializeBullet(InDirection, InSpeed);  // �Ѿ��� �ӵ��� ������ �ʱ�ȭ
    FVector StartLocation = GetActorLocation();
    SetActorLocation(StartLocation);  // �߻� ��ġ ����
    bIsHit = false;  // �Ѿ��� �߻��� ���� �浹�� �߻����� �ʾ����� ��Ÿ���� �÷��� ����
}

void ABullet::HandleBulletLifeTime(float DeltaTime)
{
    BulletLifeTime += DeltaTime;

    if (BulletLifeTime > MaxLifeTime)
    {
        Destroy();  // �Ѿ��� �ʹ� ���� ��������� �ı�
    }
}
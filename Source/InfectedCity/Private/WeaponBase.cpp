#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

AWeaponBase::AWeaponBase()
{
    // �⺻ �� ����
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

    // �浹 �� �޽� ������Ʈ ����
    RootComponent = WeaponMesh;
    WeaponCollision->SetupAttachment(WeaponMesh);

    Damage = 10.0f; // �⺻ ������
    FireRate = 1.0f; // �⺻ �߻� �ӵ�
    bCanFire = true;  // �ʱ⿡�� �߻簡 �����ϵ��� ����
}

void AWeaponBase::Fire()
{
    if (bCanFire)
    {
        // �Ѿ� �߻�
        if (BulletClass)
        {
            // �Ѿ� �߻� ���� ����
            FVector ForwardDirection = WeaponMesh->GetForwardVector();
            FActorSpawnParameters SpawnParams;
            ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation(), GetActorRotation(), SpawnParams);

            if (NewBullet)
            {
                NewBullet->InitializeBullet(ForwardDirection, 1000.0f); // �̵� �ӵ� ����
            }
        }

        // �߻� ���� ���
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // �߻� ����Ʈ ����
        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, GetActorLocation(), FRotator::ZeroRotator, true);
        }

        // �߻� �� Ÿ�̸ӷ� �߻� ���� ���·� ��ȯ
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AWeaponBase::ResetFire, FireRate, false);
    }
}

void AWeaponBase::ResetFire()
{
    bCanFire = true;  // �߻� ���� ���·� ����
}
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

AWeaponBase::AWeaponBase()
{
    // 기본 값 설정
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

    // 충돌 및 메시 컴포넌트 설정
    RootComponent = WeaponMesh;
    WeaponCollision->SetupAttachment(WeaponMesh);

    Damage = 10.0f; // 기본 데미지
    FireRate = 1.0f; // 기본 발사 속도
    bCanFire = true;  // 초기에는 발사가 가능하도록 설정
}

void AWeaponBase::Fire()
{
    if (bCanFire)
    {
        // 총알 발사
        if (BulletClass)
        {
            // 총알 발사 방향 설정
            FVector ForwardDirection = WeaponMesh->GetForwardVector();
            FActorSpawnParameters SpawnParams;
            ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation(), GetActorRotation(), SpawnParams);

            if (NewBullet)
            {
                NewBullet->InitializeBullet(ForwardDirection, 1000.0f); // 이동 속도 설정
            }
        }

        // 발사 사운드 재생
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // 발사 이펙트 생성
        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, GetActorLocation(), FRotator::ZeroRotator, true);
        }

        // 발사 후 타이머로 발사 가능 상태로 전환
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AWeaponBase::ResetFire, FireRate, false);
    }
}

void AWeaponBase::ResetFire()
{
    bCanFire = true;  // 발사 가능 상태로 변경
}
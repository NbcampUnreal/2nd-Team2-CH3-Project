#include "AkWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"  

AAkWeapon::AAkWeapon()
{
    // 기본 값 설정
    FireSoundOverride = nullptr;
    FireEffectOverride = nullptr;
}

void AAkWeapon::Fire()
{
    if (bCanFire)
    {
        // 총알 발사
        if (BulletClass)
        {
            FVector ForwardDirection = WeaponMesh->GetForwardVector();  // 발사 방향

            FActorSpawnParameters SpawnParams;
            ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation(), GetActorRotation(), SpawnParams);

            if (NewBullet)
            {
                NewBullet->InitializeBullet(ForwardDirection, 1000.0f);  // 총알 발사
            }
        }

        // 발사 사운드 처리 (override된 사운드가 있으면 그걸 사용)
        if (FireSoundOverride)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSoundOverride, GetActorLocation());
        }
        else if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // 발사 이펙트 처리 (override된 이펙트가 있으면 그걸 사용)
        if (FireEffectOverride)
        {
            // FireEffectOverride가 나이아가라 파티클 시스템이라고 가정
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffectOverride, GetActorLocation(), GetActorRotation(), FVector(1.f, 1.f, 1.f), true);
        }
        else if (FireEffect)
        {
            // FireEffect가 나이아가라 파티클 시스템이라고 가정
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffect, GetActorLocation(), GetActorRotation(), FVector(1.f, 1.f, 1.f), true);
        }

        // 발사 후 타이머로 발사 가능 상태로 전환
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AWeaponBase::ResetFire, FireRate, false);
    }
}
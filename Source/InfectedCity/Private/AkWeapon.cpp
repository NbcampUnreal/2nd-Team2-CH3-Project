#include "AkWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"  

AAkWeapon::AAkWeapon()
{
    // �⺻ �� ����
    FireSoundOverride = nullptr;
    FireEffectOverride = nullptr;
}

void AAkWeapon::Fire()
{
    if (bCanFire)
    {
        // �Ѿ� �߻�
        if (BulletClass)
        {
            FVector ForwardDirection = WeaponMesh->GetForwardVector();  // �߻� ����

            FActorSpawnParameters SpawnParams;
            ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation(), GetActorRotation(), SpawnParams);

            if (NewBullet)
            {
                NewBullet->InitializeBullet(ForwardDirection, 1000.0f);  // �Ѿ� �߻�
            }
        }

        // �߻� ���� ó�� (override�� ���尡 ������ �װ� ���)
        if (FireSoundOverride)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSoundOverride, GetActorLocation());
        }
        else if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // �߻� ����Ʈ ó�� (override�� ����Ʈ�� ������ �װ� ���)
        if (FireEffectOverride)
        {
            // FireEffectOverride�� ���̾ư��� ��ƼŬ �ý����̶�� ����
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffectOverride, GetActorLocation(), GetActorRotation(), FVector(1.f, 1.f, 1.f), true);
        }
        else if (FireEffect)
        {
            // FireEffect�� ���̾ư��� ��ƼŬ �ý����̶�� ����
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffect, GetActorLocation(), GetActorRotation(), FVector(1.f, 1.f, 1.f), true);
        }

        // �߻� �� Ÿ�̸ӷ� �߻� ���� ���·� ��ȯ
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AWeaponBase::ResetFire, FireRate, false);
    }
}
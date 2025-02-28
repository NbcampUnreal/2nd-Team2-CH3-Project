#include "WeaponBase.h"
#include "HUDWidget.h"
#include "CustomHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "InfectedCity/InfectedCityCharacter.h"

AWeaponBase::AWeaponBase()
{
    // �⺻ �� ����
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

    MaxAmmo = 30;  // ���÷� �⺻ ź�� ���� 30���� ����
    CurrentAmmo = MaxAmmo;  // �ʱ� ź���� MaxAmmo�� ����
    ReloadTime = 4.5f;


    WeaponCollision->SetupAttachment(WeaponMesh);

    Damage = 10.0f; // �⺻ ������

    bCanFire = true;  // �ʱ⿡�� �߻簡 �����ϵ��� ����

}

void AWeaponBase::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot fire while reloading!"));
        return; // ���ε� �߿��� ���� ���� �ʵ��� ��ȯ
    }

    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Firing weapon"));

        // ź�� ����
        CurrentAmmo--;

        UE_LOG(LogTemp, Log, TEXT("UpdateHUD() called!"));

        UE_LOG(LogTemp, Log, TEXT("Current Ammo: %d"), CurrentAmmo);
        // �ѱ� ������ ��ġ�� ȸ�� �� ���
        FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Bullet"));  // ����ƽ �޽����� ������ �����Ƿ�, �̸� ó���Ϸ��� �ٸ� ����� ã�ƾ� �մϴ�.
        FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(TEXT("Bullet"));  // ���������� ������ ������ �ٸ� ������� ó���ؾ� �մϴ�.

        // �߻� ���� ���
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // �߻� ����Ʈ ���� (�ѱ� ��ġ���� ����)
        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, MuzzleLocation, MuzzleRotation, true);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
    }
}

void AWeaponBase::Reloading()
{
    if (CurrentAmmo < MaxAmmo)
    {
        UE_LOG(LogTemp, Log, TEXT("Reloading..."));
        bCanFire = false;
        bIsReloading = true;
        AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(GetOwner());
        if (Player)
        {
            Player->UpdateReloadText(true);
        }
        FTimerHandle ReloadTimer;
        GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::CompleteReload, ReloadTime, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Ammo is already full!"));
    }
}

// ������ �Ϸ�
void AWeaponBase::CompleteReload()
{
    CurrentAmmo = MaxAmmo;
    UE_LOG(LogTemp, Log, TEXT("Reload complete."));
    bIsReloading = false;
    bCanFire = true;


    AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(GetOwner());
    if (Player)
    {
        UE_LOG(LogTemp, Log, TEXT("Reload ui update."));
        Player->UpdateAmmoBar();  // ź�� UI ������Ʈ
        Player->UpdateReloadText(false);
    }
}

// SetAmmo: ź�� ���� �Լ�
void AWeaponBase::SetAmmo(int32 AmmoAmount)
{
    CurrentAmmo = AmmoAmount;
    if (CurrentAmmo > MaxAmmo)
    {
        CurrentAmmo = MaxAmmo; // �ִ� ź�� ���� ���� �ʵ��� ����
    }

}

// GetCurrentAmmo: ���� ź�� ��ȯ �Լ�
int32 AWeaponBase::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

// IsOutOfAmmo: ź���� ������ Ȯ���ϴ� �Լ�
bool AWeaponBase::IsOutOfAmmo() const
{
    return CurrentAmmo <= 0;
}

bool AWeaponBase::GetIsReloading() const
{
    return bIsReloading;
}


float AWeaponBase::GetAmmoRatio() const
{
    if (MaxAmmo > 0)
    {
        return (float)CurrentAmmo / (float)MaxAmmo;
    }
    return 0.f;  // Ammo�� 0�� ��� 0 ��ȯ
}
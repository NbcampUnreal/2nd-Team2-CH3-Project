#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Components/PointLightComponent.h"
#include "Infectedcity/InfectedCityCharacter.h"

AWeaponBase::AWeaponBase()
{
    Flashlight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Flashlight1"));
    Flashlight1->SetupAttachment(RootComponent);
    Flashlight1->Intensity = 5000.f; // ��� ����
    Flashlight1->SetVisibility(false);   // �⺻������ ��������

    // ����Ʈ����Ʈ 2
    Flashlight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("Flashlight2"));
    Flashlight2->SetupAttachment(RootComponent);
    Flashlight2->Intensity = 5000.f; // ��� ����
    Flashlight2->SetVisibility(false);   // �⺻������ ��������

    bAreFlashlightsOn = false;

    
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

    MaxAmmo = 30;  // ���÷� �⺻ ź�� ���� 30���� ����6
    CurrentAmmo = MaxAmmo;  // �ʱ� ź���� MaxAmmo�� ����
    ReloadTime = 4.5f;


    WeaponCollision->SetupAttachment(WeaponMesh);

    Damage = 10.0f; // �⺻ ������

    bCanFire = true;  // �ʱ⿡�� �߻簡 �����ϵ��� ����
    PrimaryActorTick.bCanEverTick = true;
}

void AWeaponBase::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot fire while reloading!"));
        return; // ���ε� �߿��� �߻����� ����
    }

    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Firing weapon"));

        // ź�� ����
        CurrentAmmo--;
        UE_LOG(LogTemp, Log, TEXT("Current Ammo: %d"), CurrentAmmo);

        // �ѱ� ������ ��ġ�� ȸ�� �� ���
        FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Bullet"));
        FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(TEXT("Bullet"));

        // �߻� ���� ���
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // �߻� ����Ʈ ����
        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, MuzzleLocation, MuzzleRotation, true);
        }

        // �߻� �� �ݵ� ���� (�̰����� ���� �ݵ��� ó��)
        AInfectedCityCharacter* Character = Cast<AInfectedCityCharacter>(GetOwner());
        if (Character)
        {
            // ���� �ݵ� (ī�޶� �ݵ� ����)
            Character->CameraRecoil += FVector(0, 0, RecoilAmount);  // ���÷� Z�� �ݵ��� �߰�
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
        if (ReloadAnim && WeaponMesh)
        {
            WeaponMesh->PlayAnimation(ReloadAnim, false);
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
void AWeaponBase::ToggleFlashlights(bool bTurnOn)
{
    bAreFlashlightsOn = bTurnOn;

    // ����Ʈ����Ʈ 1�� 2�� �����ϰ� ���
    if (Flashlight1)
    {
        Flashlight1->SetVisibility(bAreFlashlightsOn);
    }

    if (Flashlight2)
    {
        Flashlight2->SetVisibility(bAreFlashlightsOn);
    }
}

bool AWeaponBase::AreFlashlightsOn() const
{
    return bAreFlashlightsOn;
}
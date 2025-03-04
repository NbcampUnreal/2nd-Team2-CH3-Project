#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Infectedcity/InfectedCityCharacter.h"

AWeaponBase::AWeaponBase()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    // 기본 값 설정
   
    
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));

    MaxAmmo = 30;  // 예시로 기본 탄약 수를 30으로 설정6
    CurrentAmmo = MaxAmmo;  // 초기 탄약은 MaxAmmo로 설정
    ReloadTime = 4.5f;


    WeaponCollision->SetupAttachment(WeaponMesh);

    Damage = 10.0f; // 기본 데미지

    bCanFire = true;  // 초기에는 발사가 가능하도록 설정
    PrimaryActorTick.bCanEverTick = true;
}

void AWeaponBase::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot fire while reloading!"));
        return; // 리로딩 중에는 발사하지 않음
    }

    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Firing weapon"));

        // 탄약 차감
        CurrentAmmo--;
        UE_LOG(LogTemp, Log, TEXT("Current Ammo: %d"), CurrentAmmo);

        // 총구 소켓의 위치와 회전 값 얻기
        FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Bullet"));
        FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(TEXT("Bullet"));

        // 발사 사운드 재생
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // 발사 이펙트 생성
        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, MuzzleLocation, MuzzleRotation, true);
        }

        // 발사 후 반동 적용 (이곳에서 무기 반동을 처리)
        AInfectedCityCharacter* Character = Cast<AInfectedCityCharacter>(GetOwner());
        if (Character)
        {
            // 무기 반동 (카메라에 반동 적용)
            Character->CameraRecoil += FVector(0, 0, RecoilAmount);  // 예시로 Z축 반동을 추가
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

// 재장전 완료
void AWeaponBase::CompleteReload()
{
    CurrentAmmo = MaxAmmo;
    UE_LOG(LogTemp, Log, TEXT("Reload complete."));
    bIsReloading = false;
    bCanFire = true;
}

// SetAmmo: 탄약 설정 함수
void AWeaponBase::SetAmmo(int32 AmmoAmount)
{
    CurrentAmmo = AmmoAmount;
    if (CurrentAmmo > MaxAmmo)
    {
        CurrentAmmo = MaxAmmo; // 최대 탄약 수를 넘지 않도록 설정
    }

}

// GetCurrentAmmo: 현재 탄약 반환 함수
int32 AWeaponBase::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

// IsOutOfAmmo: 탄약이 없는지 확인하는 함수
bool AWeaponBase::IsOutOfAmmo() const
{
    return CurrentAmmo <= 0;
}

bool AWeaponBase::GetIsReloading() const
{
    return bIsReloading;
}

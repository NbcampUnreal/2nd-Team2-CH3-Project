#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// 생성자
AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 무기 메쉬 생성
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    // 스페어 컬리전 컴포넌트 생성
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
    WeaponCollision->SetupAttachment(WeaponMesh);  // 메쉬에 부착
    WeaponCollision->InitSphereRadius(50.0f);      // 스피어 반지름 설정
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 물리 및 쿼리 활성화
    WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널 무시
    WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn과만 충돌 처리

    // 기본 값 설정
    Damage = 10.0f;
    FireRate = 1.0f;
    WeaponSocketName = TEXT("WeaponSocket");
}

void AWeaponBase::InitializeWeapon()
{
    // 무기 초기화 작업 (예시)
    UE_LOG(LogTemp, Log, TEXT("Weapon initialized: %s"), *GetName());
}

void AWeaponBase::Fire()
{
    // 무기 발사 동작 (예시)
    UE_LOG(LogTemp, Log, TEXT("Firing weapon: %s"), *GetName());
}

void AWeaponBase::SetWeaponMesh(UStaticMesh* NewMesh)
{
    if (WeaponMesh && NewMesh)
    {
        WeaponMesh->SetStaticMesh(NewMesh);
    }
}

void AWeaponBase::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}

void AWeaponBase::SetFireRate(float NewFireRate)
{
    FireRate = NewFireRate;
}
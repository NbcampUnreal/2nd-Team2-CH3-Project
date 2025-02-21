#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// ������
AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // ���� �޽� ����
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    // ����� �ø��� ������Ʈ ����
    WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
    WeaponCollision->SetupAttachment(WeaponMesh);  // �޽��� ����
    WeaponCollision->InitSphereRadius(50.0f);      // ���Ǿ� ������ ����
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ���� �� ���� Ȱ��ȭ
    WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� ä�� ����
    WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn���� �浹 ó��

    // �⺻ �� ����
    Damage = 10.0f;
    FireRate = 1.0f;
    WeaponSocketName = TEXT("WeaponSocket");
}

void AWeaponBase::InitializeWeapon()
{
    // ���� �ʱ�ȭ �۾� (����)
    UE_LOG(LogTemp, Log, TEXT("Weapon initialized: %s"), *GetName());
}

void AWeaponBase::Fire()
{
    // ���� �߻� ���� (����)
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
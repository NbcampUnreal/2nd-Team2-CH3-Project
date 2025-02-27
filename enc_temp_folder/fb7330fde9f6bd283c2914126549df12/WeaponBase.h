#pragma once

#include "CoreMinimal.h"
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h" 
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USoundBase;
class UParticleSystem;

UCLASS()
class INFECTEDCITY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	

public:
    AWeaponBase();

    // �߻� �Լ�
    void Fire();

    // ������ �Լ�
    void Reloading();

    // ź�� ���� �Լ�
    void SetAmmo(int32 AmmoAmount);

    // ���� ź�� ��ȯ �Լ�
    int32 GetCurrentAmmo() const;

    // ź���� �������� Ȯ���ϴ� �Լ�
    bool IsOutOfAmmo() const;
    // ���ε� �� ����
    bool bIsReloading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMeshComponent* WeaponMesh; 
    // �浹 ������Ʈ
    UPROPERTY(VisibleAnywhere)
    USphereComponent* WeaponCollision;

    // �ִ� ź�� ��
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 MaxAmmo;

    // ���� ź�� ��
    int32 CurrentAmmo;

    // ������ �ð�
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float ReloadTime;

    // ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float Damage;

    // �߻� ���� ����
    bool bCanFire;
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* ReloadAnimation; // ������ �ִϸ��̼�
    // �߻� ����
    UPROPERTY(EditAnywhere, Category = "Weapon")
    USoundBase* FireSound;

    // �߻� ����Ʈ
    UPROPERTY(EditAnywhere, Category = "Weapon")
    UParticleSystem* FireEffect;

    // ������ �Ϸ� �� ȣ��Ǵ� �Լ�
    void CompleteReload();
};
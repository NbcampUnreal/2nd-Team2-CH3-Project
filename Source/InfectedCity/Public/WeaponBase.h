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
    // ������
    AWeaponBase();
  

    void ResetFire();

protected:
    // ���� �޽� (��: ���� 3D ��)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // ���� �浹 ������Ʈ (��: ���� �浹ü)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // ���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    // �߻� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSound;

    // �߻� ����Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffect;


    // �߻� ���� ����
    bool bCanFire;

    // �Ѿ� Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<class ABullet> BulletClass;


    // �߻� Ÿ�̸� �ڵ�
    FTimerHandle FireRateTimerHandle;

public:
    // �Ѿ� �߻� �Լ�
    virtual void Fire();
};
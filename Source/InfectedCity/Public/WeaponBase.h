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

    

    // ���� �߻� �Լ�
    void Fire();

    // �߻� �� Ÿ�̸Ӱ� ������ �߻� ���� ���·� �ǵ����� �Լ�
    void ResetFire();

 

 
protected:
    // ���� �޽�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // ���� �浹
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // ���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    // �߻� �ӵ� (�ʴ� �߻� Ƚ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    // �߻� ���� ����
    bool bCanFire;

    // �߻� ������ ���� Ÿ�̸� �ڵ�
    FTimerHandle FireRateTimerHandle;

    // �Ѿ� Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABullet> BulletClass;  // ABullet�� ���

    // �߻� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSound;

    // �߻� ����Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffect;
};
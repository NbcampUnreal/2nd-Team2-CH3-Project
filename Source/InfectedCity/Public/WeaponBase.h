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
    // 생성자
    AWeaponBase();
  

    void ResetFire();

protected:
    // 무기 메시 (예: 총의 3D 모델)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // 무기 충돌 컴포넌트 (예: 구형 충돌체)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // 총의 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    // 발사 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSound;

    // 발사 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffect;


    // 발사 가능 여부
    bool bCanFire;

    // 총알 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<class ABullet> BulletClass;


    // 발사 타이머 핸들
    FTimerHandle FireRateTimerHandle;

public:
    // 총알 발사 함수
    virtual void Fire();
};
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

    

    // 무기 발사 함수
    void Fire();

    // 발사 후 타이머가 끝나면 발사 가능 상태로 되돌리는 함수
    void ResetFire();

 

 
protected:
    // 무기 메시
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // 무기 충돌
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // 무기 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    // 발사 속도 (초당 발사 횟수)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    // 발사 가능 여부
    bool bCanFire;

    // 발사 간격을 위한 타이머 핸들
    FTimerHandle FireRateTimerHandle;

    // 총알 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABullet> BulletClass;  // ABullet을 사용

    // 발사 사운드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSound;

    // 발사 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffect;
};
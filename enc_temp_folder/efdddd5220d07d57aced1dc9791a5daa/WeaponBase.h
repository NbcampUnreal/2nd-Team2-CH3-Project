#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h" 
#include "Components/SpotLightComponent.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USoundBase;
class UParticleSystem;
class UAnimMontage;

UCLASS()
class INFECTEDCITY_API AWeaponBase : public AActor
{
    GENERATED_BODY()


public:
    AWeaponBase();

    // 발사 함수
    void Fire();

    // 재장전 함수
    void Reloading();

    // 탄약 설정 함수
    void SetAmmo(int32 AmmoAmount);

    // 현재 탄약 반환 함수
    int32 GetCurrentAmmo() const;

    // 탄약이 부족한지 확인하는 함수
    bool IsOutOfAmmo() const;

    UFUNCTION(BlueprintCallable)
    bool GetIsReloading() const;

    // 리로딩 중 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
    bool bIsReloading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    USkeletalMeshComponent* WeaponMesh;
    // 충돌 컴포넌트
    UPROPERTY(VisibleAnywhere)
    USphereComponent* WeaponCollision;

    // 최대 탄약 수
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 MaxAmmo;

    // 현재 탄약 수
    int32 CurrentAmmo;

    // 재장전 시간
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float ReloadTime;

    // 데미지
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float Damage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
    float RecoilAmount = 5.0f;  // 기본 반동 크기
    // 발사 가능 여부
    bool bCanFire;
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* ReloadAnimation; // 재장전 애니메이션
    // 발사 사운드
    UPROPERTY(EditAnywhere, Category = "Weapon")
    USoundBase* FireSound;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Reload")
    UAnimMontage* ReloadAnim;  // 재장전 애니메이션 변수
    // 발사 이펙트
    UPROPERTY(EditAnywhere, Category = "Weapon")
    UParticleSystem* FireEffect;

    // 재장전 완료 후 호출되는 함수
    void CompleteReload();

    // 스포트라이트 두 개
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
    UPointLightComponent* Flashlight1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
    UPointLightComponent* Flashlight2;

    // 플래시라이트가 켜져 있는지 여부
    bool bAreFlashlightsOn;

    // 플래시라이트 토글
    void ToggleFlashlights(bool bTurnOn);

    // 플래시라이트 상태 반환
    bool AreFlashlightsOn() const;
    
};
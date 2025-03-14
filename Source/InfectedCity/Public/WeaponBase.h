#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h" 
#include "HUDWidget.h"
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

    UFUNCTION(BlueprintCallable)
    bool GetIsReloading() const;

    // ���ε� �� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
    bool bIsReloading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    USkeletalMeshComponent* WeaponMesh;
    USkeletalMeshComponent* GetMesh() const { return WeaponMesh; }
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
    float RecoilAmount = 5.0f;  // �⺻ �ݵ� ũ��
    // �߻� ���� ����
    bool bCanFire;

    // �߻� ����
    UPROPERTY(EditAnywhere, Category = "Weapon")
    USoundBase* FireSound;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Reload")
    UAnimMontage* ReloadAnim;  // ������ �ִϸ��̼� ����
    // �߻� ����Ʈ
    UPROPERTY(EditAnywhere, Category = "Weapon")
    UParticleSystem* FireEffect;

    // ������ �Ϸ� �� ȣ��Ǵ� �Լ�
    void CompleteReload();

    float GetAmmoRatio() const;

    UPROPERTY(VisibleAnywhere)
    USpotLightComponent* SpotLight1;

    UPROPERTY(VisibleAnywhere)
    USpotLightComponent* SpotLight2;

    // 스포트라이트를 켜고 끄는 함수
    void ToggleSpotlights(bool bIsEnabled);
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;


UCLASS()
class INFECTEDCITY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	

public:
    // 생성자
    AWeaponBase();

protected:
    // 무기 메시
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // 무기 충돌 (스페어 컬리전)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // 무기 기본 값 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName;

public:
    // 무기 초기화 함수 (예시)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void InitializeWeapon();

    // 무기 발사 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Fire();

    // 무기 메쉬 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponMesh(UStaticMesh* NewMesh);

    // 공격력 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetDamage(float NewDamage);

    // 발사 속도 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetFireRate(float NewFireRate);
};
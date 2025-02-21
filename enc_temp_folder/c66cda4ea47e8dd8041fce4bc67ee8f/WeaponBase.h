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
    // ������
    AWeaponBase();

protected:
    // ���� �޽�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // ���� �浹 (����� �ø���)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USphereComponent* WeaponCollision;

    // ���� �⺻ �� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName;

public:
    // ���� �ʱ�ȭ �Լ� (����)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void InitializeWeapon();

    // ���� �߻� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Fire();

    // ���� �޽� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponMesh(UStaticMesh* NewMesh);

    // ���ݷ� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetDamage(float NewDamage);

    // �߻� �ӵ� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetFireRate(float NewFireRate);
};
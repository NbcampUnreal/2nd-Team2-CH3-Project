// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AkWeapon.generated.h"


UCLASS()
class INFECTEDCITY_API AAkWeapon : public AWeaponBase
{
	GENERATED_BODY()


public:
    // ������
    AAkWeapon();

    // �������̵�� �Ѿ� �߻� �޼���
    virtual void Fire() override;

protected:
    // �Ѿ� �߻� ���� �߰� ���� ���� (��: �߻� ����, �߻� ����Ʈ ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSoundOverride;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffectOverride;
};
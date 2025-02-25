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
    // 생성자
    AAkWeapon();

    // 오버라이드된 총알 발사 메서드
    virtual void Fire() override;

protected:
    // 총알 발사 관련 추가 설정 가능 (예: 발사 사운드, 발사 이펙트 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USoundBase* FireSoundOverride;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* FireEffectOverride;
};
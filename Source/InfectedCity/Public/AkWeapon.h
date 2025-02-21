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

protected:
	// 탄창 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize;

	// 발사 모드 (자동/반자동)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsAutomatic;





public:
	// 발사 함수 (자동/반자동에 따라 다르게 처리)
	virtual void Fire() override;

	// AK 무기의 발사 모드 설정
	void SetFireMode(bool bIsAuto);

	// 탄창 크기 설정
	void SetMagazineSize(int32 NewSize);
};
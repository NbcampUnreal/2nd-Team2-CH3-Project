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

protected:
	// źâ ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize;

	// �߻� ��� (�ڵ�/���ڵ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsAutomatic;





public:
	// �߻� �Լ� (�ڵ�/���ڵ��� ���� �ٸ��� ó��)
	virtual void Fire() override;

	// AK ������ �߻� ��� ����
	void SetFireMode(bool bIsAuto);

	// źâ ũ�� ����
	void SetMagazineSize(int32 NewSize);
};
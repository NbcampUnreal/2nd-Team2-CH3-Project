#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AkWeapon.generated.h"

class UParticleSystem;
class USoundCue;
class UStaticMesh;  // Static Mesh Ŭ���� �߰�

UCLASS()
class INFECTEDCITY_API AAkWeapon : public AWeaponBase
{

	GENERATED_BODY()


public:
	// �⺻ ������
	AAkWeapon();

protected:
	// �� �߻� �� ��ƼŬ
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* FireParticleEffect;

	// �� �߻� �� �Ҹ�
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundCue* FireSoundEffect;


public:
	
	

};
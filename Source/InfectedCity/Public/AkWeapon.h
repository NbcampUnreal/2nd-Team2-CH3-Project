#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AkWeapon.generated.h"

class UParticleSystem;
class USoundCue;
class UStaticMesh;  // Static Mesh 클래스 추가

UCLASS()
class INFECTEDCITY_API AAkWeapon : public AWeaponBase
{

	GENERATED_BODY()


public:
	// 기본 생성자
	AAkWeapon();

protected:
	// 총 발사 시 파티클
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* FireParticleEffect;

	// 총 발사 시 소리
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundCue* FireSoundEffect;


public:
	
	

};
#include "AkWeapon.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// 생성자
AAkWeapon::AAkWeapon()
{
	// 기본 탄창 크기와 발사 모드 설정
	MagazineSize = 30;
	bIsAutomatic = true;

	
	
}



// 발사 함수 (자동/반자동 모드 처리)
void AAkWeapon::Fire()
{
	if (bIsAutomatic)
	{
		UE_LOG(LogTemp, Log, TEXT("Firing AK-47: Automatic mode, Damage: %f"), Damage);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Firing AK-47: Semi-Automatic mode, Damage: %f"), Damage);
	}

	// 실제 총알 발사 로직을 추가할 수 있음 (여기서는 로그만 출력)
}

// 발사 모드 설정
void AAkWeapon::SetFireMode(bool bIsAuto)
{
	bIsAutomatic = bIsAuto;
}

// 탄창 크기 설정
void AAkWeapon::SetMagazineSize(int32 NewSize)
{
	MagazineSize = NewSize;
}
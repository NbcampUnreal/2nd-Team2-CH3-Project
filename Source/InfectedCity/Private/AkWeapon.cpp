#include "AkWeapon.h"
#include "Kismet/GameplayStatics.h"

// 생성자
AAkWeapon::AAkWeapon()
{
    // 기본값 설정
    MagazineSize = 30;
    bIsAutomatic = true;
}


void AAkWeapon::SetFireMode(bool bIsAuto)
{
    bIsAutomatic = bIsAuto;
}

void AAkWeapon::SetMagazineSize(int32 NewSize)
{
    MagazineSize = NewSize;
}


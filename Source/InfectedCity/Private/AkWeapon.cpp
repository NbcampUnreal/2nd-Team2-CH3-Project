#include "AkWeapon.h"
#include "Kismet/GameplayStatics.h"

// ������
AAkWeapon::AAkWeapon()
{
    // �⺻�� ����
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


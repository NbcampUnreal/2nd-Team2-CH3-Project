#include "AkWeapon.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// ������
AAkWeapon::AAkWeapon()
{
	// �⺻ źâ ũ��� �߻� ��� ����
	MagazineSize = 30;
	bIsAutomatic = true;

	
	
}



// �߻� �Լ� (�ڵ�/���ڵ� ��� ó��)
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

	// ���� �Ѿ� �߻� ������ �߰��� �� ���� (���⼭�� �α׸� ���)
}

// �߻� ��� ����
void AAkWeapon::SetFireMode(bool bIsAuto)
{
	bIsAutomatic = bIsAuto;
}

// źâ ũ�� ����
void AAkWeapon::SetMagazineSize(int32 NewSize)
{
	MagazineSize = NewSize;
}
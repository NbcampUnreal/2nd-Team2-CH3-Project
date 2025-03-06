#include "FlashLight.h"
#include "Components/SphereComponent.h"  

AFlashLight::AFlashLight()
{
    // �⺻ �����ڿ��� �÷��ö���Ʈ �޽� �ʱ�ȭ
    PrimaryActorTick.bCanEverTick = true;

    // �÷��ö���Ʈ �޽� ������Ʈ �ʱ�ȭ
    FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
    RootComponent = FlashlightMesh;

    // �浹 �ڽ� ������Ʈ �ʱ�ȭ
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(RootComponent);  // �÷��ö���Ʈ �޽��� ���̱�
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // �浹 Ȱ��ȭ
    CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // �浹 ������Ʈ ���� ����
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);  // ��� ä�ο� ���� ���� ����

    // ���Ǿ��� ������ ���� (�ʿ信 ���� ����)
    CollisionSphere->SetSphereRadius(100.0f);  // ������ ũ�� ����
}

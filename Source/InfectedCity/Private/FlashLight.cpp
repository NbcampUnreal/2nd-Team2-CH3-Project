#include "FlashLight.h"
#include "Components/SphereComponent.h"  

AFlashLight::AFlashLight()
{
    // 기본 생성자에서 플래시라이트 메쉬 초기화
    PrimaryActorTick.bCanEverTick = true;

    // 플래시라이트 메쉬 컴포넌트 초기화
    FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
    RootComponent = FlashlightMesh;

    // 충돌 박스 컴포넌트 초기화
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(RootComponent);  // 플래시라이트 메쉬에 붙이기
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 충돌 활성화
    CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // 충돌 오브젝트 유형 설정
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);  // 모든 채널에 대해 차단 설정

    // 스피어의 반지름 설정 (필요에 따라 조정)
    CollisionSphere->SetSphereRadius(100.0f);  // 반지름 크기 설정
}

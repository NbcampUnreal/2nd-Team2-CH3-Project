#include "EnemyCharacter.h"
#include "Components/PostProcessComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "OutlineComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);

	OutlineComponent = CreateDefaultSubobject<UOutlineComponent>(TEXT("OutlineComponent"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	OutlineComponent->DisableOutline();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::EnableOutline(bool bEnable)
{
	bEnable == true ? OutlineComponent->EnableOutline() : OutlineComponent->DisableOutline();
}

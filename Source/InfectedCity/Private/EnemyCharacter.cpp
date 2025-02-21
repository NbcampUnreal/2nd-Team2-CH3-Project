#include "EnemyCharacter.h"
#include "Components/PostProcessComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Outline = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Outline"));
	Outline->SetupAttachment(GetMesh());
	Outline->bEnabled = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
}



void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetRenderCustomDepth(false);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::SetOutline(bool bValue)
{
	//Outline->bEnabled = bValue;
	GetMesh()->SetRenderCustomDepth(bValue);
}


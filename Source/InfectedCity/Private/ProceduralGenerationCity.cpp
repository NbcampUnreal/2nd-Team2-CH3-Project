#include "ProceduralGenerationCity.h"
#include "PCGComponent.h"

AProceduralGenerationCity::AProceduralGenerationCity()
{
	PrimaryActorTick.bCanEverTick = false;

	PCGComponent = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
}

void AProceduralGenerationCity::BeginPlay()
{
}

void AProceduralGenerationCity::Tick(float DeltaTime)
{
}

void AProceduralGenerationCity::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (PCGComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PCG Component Initialized in OnConstruction!"));
	}
}


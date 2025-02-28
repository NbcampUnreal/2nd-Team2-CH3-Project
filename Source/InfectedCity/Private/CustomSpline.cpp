#include "CustomSpline.h"
#include "Components/SplineComponent.h"

ACustomSpline::ACustomSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

void ACustomSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SplineComponent)
	{
		SplineComponent->SetClosedLoop(bClosedLoop);

		if (bClosedLoop == true)
		{
			Tags.AddUnique(TEXT("ClosedLoop"));
		}
		else
		{
			Tags.Remove(TEXT("ClosedLoop"));
		}
	}
}


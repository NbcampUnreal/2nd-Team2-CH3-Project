#include "Interactable/Light/InteractLight.h"
#include "InteractManager/InteractManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"


AInteractLight::AInteractLight()
{
	PrimaryActorTick.bCanEverTick = false;

	// RootComponent Set
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = Scene;

	// Static Mesh Set
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// Light Set
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);
	PointLight->Intensity = 1000.0f;
	PointLight->LightColor = FColor::White;
}

void AInteractLight::OnInteract()
{
	if (PointLight)
	{
		bool bIsVisible = PointLight->IsVisible();
		PointLight->SetVisibility(!bIsVisible);
	}
}

void AInteractLight::BeginPlay()
{
	Super::BeginPlay();
	
	if (UInteractManager* InteractManager = GetWorld()->GetSubsystem<UInteractManager>())
	{
		InteractManager->AddInteractActor(TEXT("WallLight"), this);
	}
}

void AInteractLight::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UInteractManager* InteractManager = GetWorld()->GetSubsystem<UInteractManager>())
	{
		InteractManager->RemoveInteractActor(TEXT("WallLight"));
	}
}
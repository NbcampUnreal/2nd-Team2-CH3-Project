#include "OutlineComponent.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UOutlineComponent::UOutlineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsItemOutline = (EOutlineType == EOutlineType::OUTLINE_ITEM);
}

void UOutlineComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UMeshComponent>(MeshComponents);

	if(EOutlineType == EOutlineType::OUTLINE_ITEM)
		StartCheckingDistance();
}

void UOutlineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DisableOutline();
	StopCheckingDistance();

	Super::EndPlay(EndPlayReason);
}

void UOutlineComponent::StartCheckingDistance()
{
	GetWorld()->GetTimerManager().SetTimer(DistanceCheckTimer, this, &UOutlineComponent::CheckPlayerDistance, CheckInterval, true);
}

void UOutlineComponent::StopCheckingDistance()
{
	GetWorld()->GetTimerManager().ClearTimer(DistanceCheckTimer);
}

void UOutlineComponent::CheckPlayerDistance()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{
		DisableOutline();
		return;
	}

	float Distance = FVector::Dist(PlayerCharacter->GetActorLocation(), GetOwner()->GetActorLocation());

	if (Distance <= ActivationDistance)
	{
		EnableOutline();
	}
	else
	{
		DisableOutline();
	}
}

void UOutlineComponent::EnableOutline()
{
	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			switch (EOutlineType)
			{
			case EOutlineType::OUTLINE_ENEMY:
				MeshComp->SetRenderCustomDepth(true);
				MeshComp->SetCustomDepthStencilValue(StencilValue);
				break;
			case EOutlineType::OUTLINE_ITEM:
				MeshComp->SetOverlayMaterial(OutlineMaterial);
				break;
			}
			
		}
	}
}

void UOutlineComponent::DisableOutline()
{
	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(false);
			MeshComp->SetOverlayMaterial(nullptr);
		}
	}
}

void UOutlineComponent::SetMaterialParam(float Thickness, FLinearColor Color)
{
	if (!OutlineMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("OutlineMaterial is null in %s"), *GetOwner()->GetName());
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(OutlineMaterial);

	if (!DynamicMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
		OutlineMaterial = DynamicMaterial;
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("Thickness"), Thickness);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void UOutlineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	bIsItemOutline = (EOutlineType == EOutlineType::OUTLINE_ITEM);
}

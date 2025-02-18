#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "InteractLight.generated.h"

class UPointLightComponent;

UCLASS()
class INFECTEDCITY_API AInteractLight : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractLight();

	virtual void OnInteract() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
	UPointLightComponent* PointLight;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

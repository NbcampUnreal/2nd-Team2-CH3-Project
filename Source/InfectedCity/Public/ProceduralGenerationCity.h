#pragma once

#include "CoreMinimal.h"
#include "CustomSpline.h"
#include "ProceduralGenerationCity.generated.h"

class UPCGComponent;

UCLASS()
class INFECTEDCITY_API AProceduralGenerationCity : public ACustomSpline
{
	GENERATED_BODY()

public:
	AProceduralGenerationCity();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, Category = "PCG")
	UPCGComponent* PCGComponent;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomSpline.generated.h"

class USplineComponent;

UCLASS()
class INFECTEDCITY_API ACustomSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomSpline();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Spline")
	bool bClosedLoop;
};

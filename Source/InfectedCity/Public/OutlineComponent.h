#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OutlineComponent.generated.h"

UENUM(BlueprintType)
enum class EOutlineType : uint8
{
	OUTLINE_ENEMY UMETA(DisplayName = "Enemy"),
	OUTLINE_ITEM UMETA(DisplayName = "Item"),
	OUTLINE_INTERACTION_ITEM UMETA(DisplayName = "InteractionItem"),
	OUTLINE_END UMETA(DisplayName = "None")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INFECTEDCITY_API UOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOutlineComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void StartCheckingDistance();

	void StopCheckingDistance();

	void CheckPlayerDistance();

	UFUNCTION(BlueprintCallable)
	void EnableOutline();

	UFUNCTION(BlueprintCallable)
	void DisableOutline();

	UFUNCTION(BlueprintCallable)
	void SetMaterialParam(float Thickness, FLinearColor Color);

	UPROPERTY(VisibleAnywhere, Category = "Outline")
	bool bEnable;

private:
	UPROPERTY(EditAnywhere, Category = "Outline")
	int32 StencilValue{ 0 };

	UPROPERTY(EditAnywhere, Category = "Outline")
	UMaterialInterface* OutlineMaterial{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Outline")
	EOutlineType EOutlineType{ EOutlineType::OUTLINE_END };

	UPROPERTY(EditAnywhere, Category = "Outline", meta = (EditCondition = "bIsItemOutline"))
	float CheckInterval{ 0.5f };

	UPROPERTY(EditAnywhere, Category = "Outline", meta = (EditCondition = "bIsItemOutline"))
	float ActivationDistance{ 300.0f };

	UPROPERTY()
	TArray<UMeshComponent*> MeshComponents;

	FTimerHandle DistanceCheckTimer;

private:

	UPROPERTY()
	bool bIsItemOutline{ false };

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};

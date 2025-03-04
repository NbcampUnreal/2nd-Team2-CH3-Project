#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyEffectManager.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class ABaseItem;

UCLASS()
class INFECTEDCITY_API AEnemyEffectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyEffectManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	ABaseItem* GetCurrentItem() { return CurrentItem; }
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Rendering")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UTextureRenderTarget2D* DepthRenderTarget;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UMaterialInterface* PostProcessMaterial;

	void SetupSceneCapture();
	void DetectActorAtMouseCursor(APlayerController* PlayerController);
	void DetectActorAtCenter(APlayerController* PlayerController);
	void CheckPlayerRaytrace();


	class AGN_Character* EnemyCharacter{ nullptr };
	ABaseItem* CurrentItem{ nullptr };
	bool bPreMouseCursor{ false };


};

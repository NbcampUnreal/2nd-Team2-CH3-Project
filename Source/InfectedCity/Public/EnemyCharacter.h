#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UOutlineComponent;

UCLASS()
class INFECTEDCITY_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EnableOutline(bool bEnable);

private:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UOutlineComponent* OutlineComponent;
};

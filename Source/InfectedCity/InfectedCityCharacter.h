#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "InfectedCityCharacter.generated.h"

class UHUDWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AInfectedCityCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	AInfectedCityCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY()
	UHUDWidget* HUDWidget;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;


	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
  
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; 

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;
  
	UPROPERTY()
	UHUDWidget* HUDWidget;

public:
	AInfectedCityCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();


	virtual void BeginPlay() override;

	void PickupWeapon();
	// Input setup for camera switching
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnRightMouseButtonPressed();

	void OnVKeyPressed();
	

protected:
	// 무기 주울 때의 동작
	void PickupWeapon();


	// 가까운 무기를 찾는 함수
	virtual void NotifyControllerChanged() override;

	AWeaponBase* FindNearestWeapon();
};
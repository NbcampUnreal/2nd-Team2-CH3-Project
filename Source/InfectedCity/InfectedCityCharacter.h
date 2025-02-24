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
class UHUDWidget;

UCLASS(config=Game)
class AInfectedCityCharacter : public ACharacter
{
	GENERATED_BODY()

	
public:
	// 기본 ?�성??
	AInfectedCityCharacter();

protected:


	UPROPERTY()
	UHUDWidget* HUDWidget;
	// 카메??�?(캐릭???�에 카메???�치 조정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	// 캐릭???�에 ?�라?�는 카메??

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SecondCameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* SecondFollowCamera;
	



	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
  
	UPROPERTY(EditAnywhere, Category = "Input")




	UInputAction* JumpAction; // ?�프 ?�션

	// 무기�?주울 ???�용???�력 ?�션

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;
  
	UPROPERTY()
	UHUDWidget* HUDWidget;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 캐릭?�의 ?�동 관???�력 ?�션??
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	// 캐릭?�의 ?�점 조정 ?�력 ?�션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	// ?�리�?�??�기 관???�션

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimingAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomedFOV = 45.0f;


	UPROPERTY(EditAnywhere, Category = "Camera")
	float DefaultFOV = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomInterpSpeed = 10.0f;
	bool bIsAiming = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	virtual void Tick(float DeltaTime) override;
	


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 컨트롤러가 변경될 ???�출?�는 ?�수
	virtual void NotifyControllerChanged() override;

	// 캐릭?�의 ?�동 처리 ?�수
	void Move(const FInputActionValue& Value);

	// 캐릭?�의 ?�점 조정 처리 ?�수
	void Look(const FInputActionValue& Value);

	// ?�리�? ?�기 기능
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

	// 무기 주울 ?�의 ?�작
	void PickupWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool BHASRifle() const;
	

	// �㸮 ���� ������ ���� ���� (0 = �� �ִ� ����, 1 = ������ ���� ����)
	float CrouchBlendFactor = 0.0f;

	// Input setup for camera switching
	void OnRightMouseButtonPressed();


	// Called when V key is pressed
	void OnVKeyPressed();
	
	// 가까운 무기�?찾는 ?�수
	AWeaponBase* FindNearestWeapon();

	// **���� �� �� �߻� ���**
	void StartAiming();
	void StopAiming();
	void StartShoot();
	void StopShoot();

	void RotateCharacterToMouseCursor();
};
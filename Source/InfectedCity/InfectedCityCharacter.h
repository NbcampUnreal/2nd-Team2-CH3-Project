#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InfectedCityCharacter.generated.h"

class UHUDWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AWeaponBase;
class ABullet;
class UPointLightComponent;


UCLASS(config = Game)
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




	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; // ?�프 ?�션

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;




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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction;


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
	bool bIsFiring;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	virtual void BeginPlay() override;



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* EquippedWeapon;  // ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShootAnimMontage;
	// ������ ���� ��ȯ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }  // ������ ���⸦ ��ȯ�ϴ� �Լ�
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;  // �Ѿ� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadAnimMontage;  // ������ �ִϸ��̼� ��Ÿ�� ���� �߰�

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletSpeed = 10000.f;  // �Ѿ� �ӵ�
	void FireBullet();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	// Zoomed view�� �⺻ Zoom �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultArmLength = 600.0f;  // �⺻ �������� ���� (��: 400)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomedArmLength = 600.0f;  // ���� �� �������� ���� (��: 600)

	virtual void Tick(float DeltaTime) override;
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FlashlightAction;

	// �÷��ö���Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	class UPointLightComponent* Flashlight;

	// �÷��ö���Ʈ ��� �Լ�
	void ToggleFlashlight();
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
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool BHASPistol() const;

	// �㸮 ���� ������ ���� ���� (0 = �� �ִ� ����, 1 = ������ ���� ����)
	float CrouchBlendFactor = 0.0f;



	// 가까운 무기�?찾는 ?�수
	AWeaponBase* FindNearestWeapon();

	// **���� �� �� �߻� ���**
	void StartAiming();
	void StopAiming();
	void StartShoot();
	void StopShoot();

	void RotateCharacterToMouseCursor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAmount = 10000.0f;  // �⺻ �ݵ� ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Recoil")
	float RecoilRotationAmount = 2.0f;  // �⺻��: 2�� ������ �ݵ�
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilSpeed = 10.0f;  // �ݵ� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRecoverySpeed = 5.0f;  // �ݵ� ȸ�� �ӵ�
	
	FVector CameraRecoil; // ī�޶� �ݵ�
	FRotator CameraRecoilRotation;  // ī�޶� �ݵ� �ӵ�
	void RecoverCameraRecoil();
	FVector OriginalCameraLocation;
	FRotator OriginalCameraRotation;

private:
	float LastFireTime = 0.0f;  // ������ �߻� �ð�
	float FireRate = 0.1f;      // �߻� �ӵ� (�� ����)
	
	
};
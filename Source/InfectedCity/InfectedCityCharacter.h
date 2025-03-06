#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemBase.h"
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
class AEnemyEffectManager;
class AFlashLight;

UCLASS(config = Game)
class AInfectedCityCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AInfectedCityCharacter();

	void DrainStamina();
	void RecoverStamina();
	void PickupItem();
	AEnemyEffectManager* EnemyEffectManager{ nullptr };
	int32 BandageCount{ 0 };
	int32 PillCount{ 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	int32 GasCount = 0;

	UPROPERTY()
	UHUDWidget* HUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SecondCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* SecondFollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupFlashlightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

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

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupItemAction;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomedFOV = 360.0f;


	UPROPERTY(EditAnywhere, Category = "Camera")
	float DefaultFOV = 360.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomInterpSpeed = 10.0f;
	bool bIsAiming = false;
	bool bIsFiring;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HitAnimMontage;
public:
	void UpdateAmmoBar();

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


	// �÷��ö���Ʈ ��� �Լ�
	void ToggleFlashlight();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

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

	UFUNCTION(BlueprintCallable)
	void UpdateReloadText(bool bIsReloading);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, int32> Inventory;


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemClass, int32 Amount);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(FName ItemClass);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP;

	float Stamina;
	const float MaxStamina = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* DeathAnimSequence;  

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHP(float NewHP);

	UFUNCTION(BlueprintCallable, Category = "RideSystem")
	void OnRideAvailable();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "State")
	void DeathEvent();

private:
	float LastFireTime = 0.0f;
	float FireRate = 0.1f;

	const float StaminaDrainRate = 20.0f;
	const float StaminaRecoveryRate = 10.0f;
	bool bCanRun = true;
	FTimerHandle DeathAnimTimerHandle;
	FTimerHandle StaminaTimerHandle;

public:
		// 플레이어의 현재 체력과 최대 체력
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth;

		// 체력 변경 함수
		virtual float TakeDamage(float DamageAmount);

		void Die();

		bool bIsDead;
		
		void HandleDeath();
		bool bIsPlayingHitAnim;

		// 애니메이션 상태 리셋 함수
		void ResetHitAnimState();
	UUserWidget* CurrentUIWidget;

};

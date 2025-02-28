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


UCLASS(config = Game)
class AInfectedCityCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// 湲곕낯 ?앹꽦??
	AInfectedCityCharacter();

protected:


	UPROPERTY()
	UHUDWidget* HUDWidget;
	// 移대찓??遺?(罹먮┃???ㅼ뿉 移대찓???꾩튂 議곗젙)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	// 罹먮┃???ㅼ뿉 ?곕씪?ㅻ뒗 移대찓??

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SecondCameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* SecondFollowCamera;




	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; // ?먰봽 ?≪뀡

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;

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
	void UpdateAmmoBar();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* EquippedWeapon;  // 장착된 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShootAnimMontage;
	// 장착된 무기 반환 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }  // 장착된 무기를 반환하는 함수
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;  // 총알 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadAnimMontage;  // 재장전 애니메이션 몽타주 변수 추가

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletSpeed = 10000.f;  // 총알 속도
	void FireBullet();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	// Zoomed view와 기본 Zoom 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultArmLength = 600.0f;  // 기본 스프링암 길이 (예: 400)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomedArmLength = 600.0f;  // 줌인 시 스프링암 길이 (예: 600)

	virtual void Tick(float DeltaTime) override;
	void Reload();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();
	void PickupWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool BHASRifle() const;


	// 허리 숙임 보간을 위한 변수 (0 = 서 있는 상태, 1 = 완전히 숙인 상태)
	float CrouchBlendFactor = 0.0f;

	AWeaponBase* FindNearestWeapon();

	// **줌인 및 총 발사 기능**
	void StartAiming();
	void StopAiming();
	void StartShoot();
	void StopShoot();

	void RotateCharacterToMouseCursor();

	UFUNCTION(BlueprintCallable)
	void UpdateReloadText(bool bIsReloading);


private:
	float LastFireTime = 0.0f;  // 마지막 발사 시간
	float FireRate = 0.1f;      // 발사 속도 (초 단위)
};
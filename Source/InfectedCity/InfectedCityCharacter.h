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
	// ê¸°ë³¸ ?ì„±??
	AInfectedCityCharacter();

protected:


	UPROPERTY()
	UHUDWidget* HUDWidget;
	// ì¹´ë©”??ë¶?(ìºë¦­???¤ì— ì¹´ë©”???„ì¹˜ ì¡°ì •)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	// ìºë¦­???¤ì— ?°ë¼?¤ëŠ” ì¹´ë©”??

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SecondCameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* SecondFollowCamera;




	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; // ?í”„ ?¡ì…˜

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;




	// ìºë¦­?°ì˜ ?´ë™ ê´€???…ë ¥ ?¡ì…˜??
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	// ìºë¦­?°ì˜ ?œì  ì¡°ì • ?…ë ¥ ?¡ì…˜
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	// ?¬ë¦¬ê¸?ë°??‰ê¸° ê´€???¡ì…˜

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
	AWeaponBase* EquippedWeapon;  // ÀåÂøµÈ ¹«±â
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShootAnimMontage;
	// ÀåÂøµÈ ¹«±â ¹İÈ¯ ÇÔ¼ö
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }  // ÀåÂøµÈ ¹«±â¸¦ ¹İÈ¯ÇÏ´Â ÇÔ¼ö
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;  // ÃÑ¾Ë Å¬·¡½º
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadAnimMontage;  // ÀçÀåÀü ¾Ö´Ï¸ŞÀÌ¼Ç ¸ùÅ¸ÁÖ º¯¼ö Ãß°¡

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletSpeed = 10000.f;  // ÃÑ¾Ë ¼Óµµ
	void FireBullet();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	// Zoomed view¿Í ±âº» Zoom °Å¸®
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultArmLength = 600.0f;  // ±âº» ½ºÇÁ¸µ¾Ï ±æÀÌ (¿¹: 400)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomedArmLength = 600.0f;  // ÁÜÀÎ ½Ã ½ºÇÁ¸µ¾Ï ±æÀÌ (¿¹: 600)

	virtual void Tick(float DeltaTime) override;
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FlashlightAction;

	// ÇÃ·¡½Ã¶óÀÌÆ® ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	class UPointLightComponent* Flashlight;

	// ÇÃ·¡½Ã¶óÀÌÆ® Åä±Û ÇÔ¼ö
	void ToggleFlashlight();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ì»¨íŠ¸ë¡¤ëŸ¬ê°€ ë³€ê²½ë  ???¸ì¶œ?˜ëŠ” ?¨ìˆ˜
	virtual void NotifyControllerChanged() override;

	// ìºë¦­?°ì˜ ?´ë™ ì²˜ë¦¬ ?¨ìˆ˜
	void Move(const FInputActionValue& Value);

	// ìºë¦­?°ì˜ ?œì  ì¡°ì • ì²˜ë¦¬ ?¨ìˆ˜
	void Look(const FInputActionValue& Value);

	// ?¬ë¦¬ê¸? ?‰ê¸° ê¸°ëŠ¥
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

	
	// ë¬´ê¸° ì£¼ìš¸ ?Œì˜ ?™ì‘
	void PickupWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool BHASRifle() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool BHASPistol() const;

	// Çã¸® ¼÷ÀÓ º¸°£À» À§ÇÑ º¯¼ö (0 = ¼­ ÀÖ´Â »óÅÂ, 1 = ¿ÏÀüÈ÷ ¼÷ÀÎ »óÅÂ)
	float CrouchBlendFactor = 0.0f;



	// ê°€ê¹Œìš´ ë¬´ê¸°ë¥?ì°¾ëŠ” ?¨ìˆ˜
	AWeaponBase* FindNearestWeapon();

	// **ÁÜÀÎ ¹× ÃÑ ¹ß»ç ±â´É**
	void StartAiming();
	void StopAiming();
	void StartShoot();
	void StopShoot();

	void RotateCharacterToMouseCursor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAmount = 10000.0f;  // ±âº» ¹İµ¿ Å©±â
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Recoil")
	float RecoilRotationAmount = 2.0f;  // ±âº»°ª: 2µµ Á¤µµÀÇ ¹İµ¿
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilSpeed = 10.0f;  // ¹İµ¿ ¼Óµµ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRecoverySpeed = 5.0f;  // ¹İµ¿ È¸º¹ ¼Óµµ
	
	FVector CameraRecoil; // Ä«¸Ş¶ó ¹İµ¿
	FRotator CameraRecoilRotation;  // Ä«¸Ş¶ó ¹İµ¿ ¼Óµµ
	void RecoverCameraRecoil();
	FVector OriginalCameraLocation;
	FRotator OriginalCameraRotation;

private:
	float LastFireTime = 0.0f;  // ¸¶Áö¸· ¹ß»ç ½Ã°£
	float FireRate = 0.1f;      // ¹ß»ç ¼Óµµ (ÃÊ ´ÜÀ§)
	
	
};
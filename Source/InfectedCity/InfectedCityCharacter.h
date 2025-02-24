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
	



	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
  
	UPROPERTY(EditAnywhere, Category = "Input")




	UInputAction* JumpAction; // ?í”„ ?¡ì…˜

	// ë¬´ê¸°ë¥?ì£¼ìš¸ ???¬ìš©???…ë ¥ ?¡ì…˜

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
	

	// Çã¸® ¼÷ÀÓ º¸°£À» À§ÇÑ º¯¼ö (0 = ¼­ ÀÖ´Â »óÅÂ, 1 = ¿ÏÀüÈ÷ ¼÷ÀÎ »óÅÂ)
	float CrouchBlendFactor = 0.0f;

	// Input setup for camera switching
	void OnRightMouseButtonPressed();


	// Called when V key is pressed
	void OnVKeyPressed();
	
	// ê°€ê¹Œìš´ ë¬´ê¸°ë¥?ì°¾ëŠ” ?¨ìˆ˜
	AWeaponBase* FindNearestWeapon();

	// **ÁÜÀÎ ¹× ÃÑ ¹ß»ç ±â´É**
	void StartAiming();
	void StopAiming();
	void StartShoot();
	void StopShoot();

	void RotateCharacterToMouseCursor();
};
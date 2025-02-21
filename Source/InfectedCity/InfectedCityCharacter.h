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
	// 기본 생성자
	AInfectedCityCharacter();

protected:
	// 카메라 붐 (캐릭터 뒤에 카메라 위치 조정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	// 캐릭터 뒤에 따라오는 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	// 캐릭터가 가지고 있는 무기
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
  
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; // 점프 액션

	// 무기를 주울 때 사용할 입력 액션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;
  
	UPROPERTY()
	UHUDWidget* HUDWidget;

public:
	// 생성자
	AInfectedCityCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 캐릭터의 이동 관련 입력 액션들
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	// 캐릭터의 시점 조정 입력 액션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	// 달리기 및 앉기 관련 액션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	// 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

public:
	// 플레이어 입력 처리 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 컨트롤러가 변경될 때 호출되는 함수
	virtual void NotifyControllerChanged() override;

	// 캐릭터의 이동 처리 함수
	void Move(const FInputActionValue& Value);

	// 캐릭터의 시점 조정 처리 함수
	void Look(const FInputActionValue& Value);

	// 달리기, 앉기 기능
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

	// 무기 주울 때의 동작
	void PickupWeapon();
	// Input setup for camera switching
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnRightMouseButtonPressed();

	// Called when V key is pressed
	void OnVKeyPressed();
	

protected:
	// 컨트롤러가 변경될 때 호출되는 함수
	virtual void NotifyControllerChanged() override;

	// 가까운 무기를 찾는 함수
	AWeaponBase* FindNearestWeapon();
};
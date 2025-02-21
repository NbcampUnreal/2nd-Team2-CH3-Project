// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InfectedCityCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AInfectedCityCharacter : public ACharacter
{
	GENERATED_BODY()

	

	/** 입력 매핑 컨텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** 점프 입력 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** 이동 입력 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** 시점 조정 입력 액션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 달리기와 앉기 입력 액션 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;  // Shift 키로 달리기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;  // Ctrl 키로 앉기

public:
	// 생성자
	AInfectedCityCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom; // The spring arm for TPS camera

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TPSCameraComponent; // TPS Camera

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FPSCameraComponent; // FPS Camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RightMouseButtonAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* VKeyAction;
	// Boolean to toggle camera modes
	bool bIsTPSCameraActive;

	// Input functions for switching between cameras
	void SwitchToTPSCamera();
	void SwitchToFPSCamera();

	/** 캐릭터 이동을 처리하는 함수 */
	void Move(const FInputActionValue& Value);

	/** 캐릭터 시점 조정을 처리하는 함수 */
	void Look(const FInputActionValue& Value);

	// 달리기, 앉기 기능
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

	

	// Input setup for camera switching
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnRightMouseButtonPressed();

	// Called when V key is pressed
	void OnVKeyPressed();
	

protected:
	// 컨트롤러가 변경될 때 호출되는 함수
	virtual void NotifyControllerChanged() override;

	

};
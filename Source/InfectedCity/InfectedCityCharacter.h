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

	

	/** �Է� ���� ���ؽ�Ʈ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** ���� �Է� �׼� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** �̵� �Է� �׼� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** ���� ���� �Է� �׼� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// �޸���� �ɱ� �Է� �׼� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;  // Shift Ű�� �޸���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;  // Ctrl Ű�� �ɱ�

public:
	// ������
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

	/** ĳ���� �̵��� ó���ϴ� �Լ� */
	void Move(const FInputActionValue& Value);

	/** ĳ���� ���� ������ ó���ϴ� �Լ� */
	void Look(const FInputActionValue& Value);

	// �޸���, �ɱ� ���
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
	// ��Ʈ�ѷ��� ����� �� ȣ��Ǵ� �Լ�
	virtual void NotifyControllerChanged() override;

	

};
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

	/** ī�޶� ĳ���� �ڿ� ��ġ��Ű�� ī�޶� �� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** ĳ���� �ڿ� ������� ī�޶� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

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
	/** ĳ���� �̵��� ó���ϴ� �Լ� */
	void Move(const FInputActionValue& Value);

	/** ĳ���� ���� ������ ó���ϴ� �Լ� */
	void Look(const FInputActionValue& Value);

	// �޸���, �ɱ� ���
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

protected:
	// ��Ʈ�ѷ��� ����� �� ȣ��Ǵ� �Լ�
	virtual void NotifyControllerChanged() override;

	// �÷��̾� �Է� ���� �Լ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** ī�޶� ���� ��ȯ�ϴ� �Լ� */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** ������� ī�޶� ��ȯ�ϴ� �Լ� */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	void CrouchCharacter();
};
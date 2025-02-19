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

	/** 카메라를 캐릭터 뒤에 위치시키는 카메라 붐 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** 캐릭터 뒤에 따라오는 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

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
	/** 캐릭터 이동을 처리하는 함수 */
	void Move(const FInputActionValue& Value);

	/** 캐릭터 시점 조정을 처리하는 함수 */
	void Look(const FInputActionValue& Value);

	// 달리기, 앉기 기능
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

protected:
	// 컨트롤러가 변경될 때 호출되는 함수
	virtual void NotifyControllerChanged() override;

	// 플레이어 입력 설정 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** 카메라 붐을 반환하는 함수 */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** 따라오는 카메라를 반환하는 함수 */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	void CrouchCharacter();
};
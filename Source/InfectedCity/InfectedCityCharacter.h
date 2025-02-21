#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "InfectedCityCharacter.generated.h"

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
	// �⺻ ������
	AInfectedCityCharacter();

protected:
	// ī�޶� �� (ĳ���� �ڿ� ī�޶� ��ġ ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	// ĳ���� �ڿ� ������� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	// ĳ���Ͱ� ������ �ִ� ����
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction; // ���� �׼�

	// ���⸦ �ֿ� �� ����� �Է� �׼�
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupWeaponAction;

	// ĳ������ �̵� ���� �Է� �׼ǵ�
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	// ĳ������ ���� ���� �Է� �׼�
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	// �޸��� �� �ɱ� ���� �׼�
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	// �Է� ���� ���ؽ�Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

public:
	// �÷��̾� �Է� ó�� �Լ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ��Ʈ�ѷ��� ����� �� ȣ��Ǵ� �Լ�
	virtual void NotifyControllerChanged() override;

	// ĳ������ �̵� ó�� �Լ�
	void Move(const FInputActionValue& Value);

	// ĳ������ ���� ���� ó�� �Լ�
	void Look(const FInputActionValue& Value);

	// �޸���, �ɱ� ���
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();

	// ���� �ֿ� ���� ����
	void PickupWeapon();

	// ����� ���⸦ ã�� �Լ�
	AWeaponBase* FindNearestWeapon();
};
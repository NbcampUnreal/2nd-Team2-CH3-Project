#include "InfectedCityCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"


AInfectedCityCharacter::AInfectedCityCharacter()
{
	// �⺻ ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ���� �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// ī�޶� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AInfectedCityCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AInfectedCityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::Look);

		// Running (Shift key)
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopRunning);

		// Crouching (Ctrl key)
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopCrouching);

		// Pickup Weapon (F key)
		EnhancedInputComponent->BindAction(PickupWeaponAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::PickupWeapon);
	}
	
}

void AInfectedCityCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInfectedCityCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AInfectedCityCharacter::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f; // Increase walk speed for running
}

void AInfectedCityCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // Reset walk speed to normal
}

void AInfectedCityCharacter::StartCrouching()
{
	Crouch(); // Make the character crouch
}

void AInfectedCityCharacter::StopCrouching()
{
	UnCrouch(); // Make the character stand up
}

void AInfectedCityCharacter::PickupWeapon()
{
	AWeaponBase* NearestWeapon = FindNearestWeapon();
	if (NearestWeapon)
	{
		// ���⸦ �ݰ�, �κ��丮�� ��� �߰�
		CurrentWeapon = NearestWeapon;

		// ������ �浹�� ��Ȱ��ȭ (����� ĳ���� ���� �浹�� ����)
		UPrimitiveComponent* WeaponComponent = Cast<UPrimitiveComponent>(CurrentWeapon->GetRootComponent());
		if (WeaponComponent)
		{
			// �浹�� ��Ȱ��ȭ�Ͽ� ĳ���Ϳ��� �浹�� ����
			WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // �浹 ��Ȱ��ȭ
		}

		// ���� ���̵��� ���� (HiddenInGame�� false�� ����)
		NearestWeapon->SetActorHiddenInGame(false);

		// ���⸦ �޼� ����(AkGun)�� ���̱�
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);  // SnapToTarget: ���� ��ġ�� ���缭 ����
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("AKGun"));  // GetMesh()�� ĳ������ Skeletal Mesh ������Ʈ

		// ���� ���� (���忡�� �����ϰų� �κ��丮�� �߰��ϴ� ���� ����)
		
		UE_LOG(LogTemp, Log, TEXT("Hold Weapon: %s"), *CurrentWeapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("There are no weapons nearby"));
	}
}

AWeaponBase* AInfectedCityCharacter::FindNearestWeapon()
{
	TArray<AActor*> NearbyWeapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponBase::StaticClass(), NearbyWeapons);

	AWeaponBase* NearestWeapon = nullptr;
	float NearestDistance = FLT_MAX;

	for (AActor* Actor : NearbyWeapons)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(Actor);
		{
			float Distance = FVector::Dist(GetActorLocation(), Weapon->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestWeapon = Weapon;
				NearestDistance = Distance;
			}
		}
	}

	return NearestWeapon;
}
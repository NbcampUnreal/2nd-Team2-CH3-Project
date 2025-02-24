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
#include "HUDWidget.h"


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


	//new camera
	SecondCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SecondCameraBoom"));
	SecondCameraBoom->SetupAttachment(RootComponent);
	SecondCameraBoom->TargetArmLength = 600.0f;  // �� ī�޶��� ���̸� ������ �ٸ��� ����
	SecondCameraBoom->bUsePawnControlRotation = true;

	SecondFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SecondFollowCamera"));
	SecondFollowCamera->SetupAttachment(SecondCameraBoom, USpringArmComponent::SocketName);
	SecondFollowCamera->bUsePawnControlRotation = false; // ī�޶�� ȸ������ ����
	

}
void AInfectedCityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
}
void AInfectedCityCharacter::BeginPlay()
{
	Super::BeginPlay();



	// �÷��̾� ��Ʈ�ѷ� ��������
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && HUDWidgetClass)
	{
		// HUD ���� ����
		HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
		else
		{
		}
	}
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

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopAiming);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StartShoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StopShoot);
	}
	
}

void AInfectedCityCharacter::StartAiming()
{
	bIsAiming = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = true;  // ���콺 Ŀ�� ���̰� ����
		RotateCharacterToMouseCursor();  // ���콺�� ���� ĳ���� ȸ��
	}

	// ���� ī�޶󿡼� ���ο� ī�޶�� ��ȯ
	FollowCamera->Deactivate();
	SecondFollowCamera->Activate();

	// ���ο� ī�޶�� �� ��
	SecondFollowCamera->SetFieldOfView(FMath::FInterpTo(SecondFollowCamera->FieldOfView, ZoomedFOV, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed));

	// ���콺�� ���� ȸ��
	RotateCharacterToMouseCursor();
}

void AInfectedCityCharacter::StopAiming()
{
	bIsAiming = false;

	// ���ο� ī�޶󿡼� ���� ī�޶�� ��ȯ
	SecondFollowCamera->Deactivate();
	FollowCamera->Activate();

	// ���� ī�޶�� �� �ƿ�
	FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, DefaultFOV, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = false;  // ���콺 Ŀ�� �����
	}
}

void AInfectedCityCharacter::StartShoot()
{

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = true;  // ���콺 Ŀ�� ���̰� ����
		RotateCharacterToMouseCursor();  // ���콺�� ���� ĳ���� ȸ��
	}
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}
void AInfectedCityCharacter::StopShoot()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = false;  // ���콺 Ŀ�� �����
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

bool AInfectedCityCharacter::BHASRifle() const
{
	
	return CurrentWeapon != nullptr && CurrentWeapon->IsA(AWeaponBase::StaticClass());  // ARifleWeapon�� ���� Ŭ������ ��Ÿ���ٰ� ����
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
    if (!GetCharacterMovement()->IsFalling())  
    {
        Crouch(); 
		if (HUDWidget)
		{
			HUDWidget->SetCrouchState(true);
		}
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Cannot crouch while jumping"));
    }
}

void AInfectedCityCharacter::StopCrouching()
{
	UnCrouch(); // Make the character stand up
	if (HUDWidget)
	{
		HUDWidget->SetCrouchState(false);
	}
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

void AInfectedCityCharacter::RotateCharacterToMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// ���� Ʈ���̽��� �����Ͽ� ���콺�� ����Ű�� ���� ã��
		FHitResult HitResult;
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = TraceStart + (WorldDirection * 5000.f); // 5000 ���� �Ÿ����� �˻�

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // �ڱ� �ڽ��� ����

		// ���� Ʈ���̽� ����
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			FVector LookAtTarget = HitResult.ImpactPoint;
			FVector CharacterLocation = GetActorLocation();
			FVector Direction = LookAtTarget - CharacterLocation;
			Direction.Z = 0; // ���� ȸ�� ����

			FRotator NewRotation = Direction.Rotation();
			SetActorRotation(NewRotation);
		}

		// ����� ���� (���� Ʈ���̽� Ȯ�ο�)
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
	}
}

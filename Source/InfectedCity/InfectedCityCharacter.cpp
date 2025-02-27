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
#include "InteractManager/InteractManager.h"
#include "Blueprint/UserWidget.h"
#include "Bullet.h"



AInfectedCityCharacter::AInfectedCityCharacter()
{
	static ConstructorHelpers::FClassFinder<ABullet> BulletClassFinder(TEXT("/Game/Blueprints/bullet/BP_Bullet"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
		UE_LOG(LogTemp, Log, TEXT("Bullet class loaded successfully"));
	}
	else
	{
		BulletClass = nullptr;
		UE_LOG(LogTemp, Error, TEXT("Failed to load Bullet class"));
	}
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	
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
	SecondCameraBoom->TargetArmLength = 600.0f;  // 새 카메라의 길이를 기존과 다르게 설정
	SecondCameraBoom->bUsePawnControlRotation = true;

	SecondFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SecondFollowCamera"));
	SecondFollowCamera->SetupAttachment(SecondCameraBoom, USpringArmComponent::SocketName);
	SecondFollowCamera->bUsePawnControlRotation = false; // 카메라는 회전하지 않음
	

}
void AInfectedCityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
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

void AInfectedCityCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
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
		// Pickup Weapon (Mouse Left key)
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopAiming);
		// Pickup Weapon (Mouse Right key)
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StartShoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StopShoot);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::Reload);
	}
	
}

void AInfectedCityCharacter::StartAiming()
{
	bIsAiming = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = true;  // 마우스 커서 보이게 설정
		RotateCharacterToMouseCursor();  // 마우스를 따라 캐릭터 회전
	}

	// 기존 카메라에서 새로운 카메라로 전환
	FollowCamera->Deactivate();
	SecondFollowCamera->Activate();

	
	// 마우스를 따라 회전
	RotateCharacterToMouseCursor();


	// 카메라 붐의 길이만 조정 (회전은 유지)
	SecondCameraBoom->TargetArmLength = FMath::FInterpTo(SecondCameraBoom->TargetArmLength, ZoomedArmLength, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);

	
}

void AInfectedCityCharacter::StopAiming()
{
	bIsAiming = false;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = false;  // 마우스 커서 숨기기
	}

	// 새로운 카메라에서 기존 카메라로 전환
	SecondFollowCamera->Deactivate();
	FollowCamera->Activate();

	// 카메라 붐의 회전 각도를 그대로 유지하고, 길이만 기본값으로 설정
	FRotator CurrentBoomRotation = CameraBoom->GetComponentRotation();
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DefaultArmLength, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);
	CameraBoom->SetWorldRotation(CurrentBoomRotation);
}

void AInfectedCityCharacter::StartShoot()
{
	// 마우스를 누르고 있으면 계속 발사
	bIsFiring = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = true; 
		RotateCharacterToMouseCursor(); 
	}

	// 마우스를 누르고 있으면 계속 발사
	if (bIsFiring && CurrentWeapon)
	{
		// 시간이 FireRate 이상이 되었을 때 발사
		if (GetWorld()->GetTimeSeconds() - LastFireTime >= FireRate)
		{
			FireBullet();

			// 무기가 AWeaponBase일 때 Fire() 함수 호출
			if (AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon))
			{
				Weapon->Fire();  // 발사 함수 호출
			}

			// 마지막 발사 시간 갱신
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
	}
}
void AInfectedCityCharacter::StopShoot()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = false;  // 마우스 커서 숨기기
	}
	bIsFiring = false;  // 발사 중지

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
	
	return CurrentWeapon != nullptr && CurrentWeapon->IsA(AWeaponBase::StaticClass());  // ARifleWeapon은 소총 클래스를 나타낸다고 가정
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

void AInfectedCityCharacter::Reload()
{
	// 현재 무기가 있다면
	if (CurrentWeapon)
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon))
		{
			Weapon->Reloading(); // 무기의 Reload 함수 호출
		}
	}

	// 재장전 애니메이션을 실행한다면
	if (ReloadAnimMontage)
	{
		PlayAnimMontage(ReloadAnimMontage);
	}
}
void AInfectedCityCharacter::PickupWeapon()
{
	AWeaponBase* NearestWeapon = FindNearestWeapon();
	if (NearestWeapon)
	{
		CurrentWeapon = NearestWeapon;

		// 무기 컴포넌트를 가져옵니다.
		UPrimitiveComponent* WeaponComponent = Cast<UPrimitiveComponent>(CurrentWeapon->GetRootComponent());
		if (WeaponComponent)
		{
			WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
		}

		// 무기를 화면에 보이게 설정
		NearestWeapon->SetActorHiddenInGame(false);

		// 'AKGun' 소켓에 무기 부착
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("AKGun"));

		

		// 무기 이름 로그 출력
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

	// 마우스 위치를 월드 좌표로 변환
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// 마우스 포인터의 위치로 바라보는 방향을 계산
		FVector LookAtTarget = WorldLocation + (WorldDirection * 5000.f);  // 큰 거리까지 가는 점을 설정 (직선으로 계산)

		// 캐릭터와 마우스 위치의 방향 벡터 계산 (Z축을 제외한 방향)
		FVector CharacterLocation = GetActorLocation();
		FVector Direction = LookAtTarget - CharacterLocation;
		Direction.Z = 0; // 상하 회전 방지 (Y축 회전만 하도록 설정)

		// 새로운 회전 값 계산
		FRotator NewRotation = Direction.Rotation();

		// 캐릭터 회전 적용
		SetActorRotation(NewRotation);
	}

	// 디버그 라인 (라인 트레이스 확인용 - 테스트 목적)
	//DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + WorldDirection * 5000.f, FColor::Red, false, 1.0f, 0, 2.0f);
}
void AInfectedCityCharacter::FireBullet()
{
	if (!BulletClass || !CurrentWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Bullet class or weapon is not valid!"));
		return;
	}

	// 현재 탄약이 없다면 발사하지 않음
	AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon);
	if (Weapon && Weapon->IsOutOfAmmo())
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
		return;
	}
	if (Weapon && Weapon->bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire while reloading!"));
		return;
	}
	// 발사 처리
	FVector MouseWorldLocation, MouseWorldDirection;
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
	{
		FVector ShootDirection = MouseWorldDirection.GetSafeNormal();
		FVector SpawnLocation = GetMesh()->GetSocketLocation("Bullet");
		FRotator SpawnRotation = ShootDirection.Rotation();

		if (ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation))
		{
			NewBullet->Fire(ShootDirection, BulletSpeed);
			
		}

		// 발사 애니메이션 실행
		if (ShootAnimMontage)
		{
			PlayAnimMontage(ShootAnimMontage);
		}
	}
}
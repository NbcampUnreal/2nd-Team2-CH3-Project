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
#include "WeaponBase.h"
#include "AkWeapon.h"
#include "Components/PointLightComponent.h"
#include "EnemyEffectManager.h"
#include "BaseItem.h"
#include "TimerManager.h"

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
	SecondCameraBoom->TargetArmLength = 600.0f;  //       
	SecondCameraBoom->bUsePawnControlRotation = true;

	SecondFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SecondFollowCamera"));
	SecondFollowCamera->SetupAttachment(SecondCameraBoom, USpringArmComponent::SocketName);
	SecondFollowCamera->bUsePawnControlRotation = false; //           

	CameraBoom->ProbeChannel = ECC_Visibility;
	CameraBoom->bDoCollisionTest = true;
	MaxHealth = 100.0f;
	Health = MaxHealth; // 체력 초기화
	DeathAnimTimerHandle = FTimerHandle();

	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	
}
void AInfectedCityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		//PC->SetMouseLocation(1920/2, 1080/2);
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

void AInfectedCityCharacter::BeginPlay()
{
	Super::BeginPlay();

	Stamina = MaxStamina;  // 스테미나 초기화

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	UpdateAmmoBar();
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
		// Pickup Weapon (Mouse Right key)
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopAiming);
		// Pickup Weapon (Mouse Left key)
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StartShoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StopShoot);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::Reload);

		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::ToggleFlashlight);

		EnhancedInputComponent->BindAction(PickupItemAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::PickupItem);

		
	}

}

void AInfectedCityCharacter::StartAiming()
{
	bIsAiming = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = true;   
		RotateCharacterToMouseCursor(); 
	}

	
	FollowCamera->Deactivate();
	SecondFollowCamera->Activate();


	RotateCharacterToMouseCursor();

	SecondCameraBoom->TargetArmLength = FMath::FInterpTo(SecondCameraBoom->TargetArmLength, ZoomedArmLength, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);


}

void AInfectedCityCharacter::StopAiming()
{
	bIsAiming = false;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = false;      
	}


	SecondFollowCamera->Deactivate();
	FollowCamera->Activate();

        
	FRotator CurrentBoomRotation = CameraBoom->GetComponentRotation();
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DefaultArmLength, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);
	CameraBoom->SetWorldRotation(CurrentBoomRotation);
}

void AInfectedCityCharacter::StartShoot()
{
	bIsFiring = true;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = true;
		RotateCharacterToMouseCursor();
	}

	if (bIsFiring && CurrentWeapon && !CurrentWeapon->IsOutOfAmmo() && !CurrentWeapon->bIsReloading)
	{
		
		RecoverCameraRecoil();
		if (GetWorld()->GetTimeSeconds() - LastFireTime >= FireRate)
		{
			FireBullet();
			
			
			if (AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon))
			{
				Weapon->Fire(); 
				UpdateAmmoBar();
			}
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
	}     
}
void AInfectedCityCharacter::StopShoot()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = false;  
	}
	bIsFiring = false;  
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
	return CurrentWeapon != nullptr && CurrentWeapon->IsA(AAkWeapon::StaticClass());
}

void AInfectedCityCharacter::StartRunning()
{
	if (bCanRun && Stamina > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.f;
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &AInfectedCityCharacter::DrainStamina, 0.1f, true);
	}
}

void AInfectedCityCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &AInfectedCityCharacter::RecoverStamina, 0.1f, true);
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
	
	// 무기를 들고 있을 때만 리로드 실행
	if (CurrentWeapon)
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon))
		{
			// 탄약이 부족할 때만 리로드 시작
			if (Weapon->CurrentAmmo < Weapon->MaxAmmo)
			{
				if (ReloadAnimMontage)
				{

				  PlayAnimMontage(ReloadAnimMontage);
				  Weapon->Reloading();
					
				}
				
			}
		}
	}
}

void AInfectedCityCharacter::ToggleFlashlight()
{
	if (CurrentWeapon)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon);
		if (Weapon)
		{
			// 스포트라이트를 현재 상태 반대로 토글
			bool bCurrentVisibility = (Weapon->SpotLight1->IsVisible() && Weapon->SpotLight2->IsVisible());
			Weapon->ToggleSpotlights(!bCurrentVisibility);
		}
	}
}
void AInfectedCityCharacter::PickupWeapon()
{
    AWeaponBase* NearestWeapon = FindNearestWeapon();
    if (NearestWeapon)
    {
        CurrentWeapon = NearestWeapon;

        UPrimitiveComponent* WeaponComponent = Cast<UPrimitiveComponent>(CurrentWeapon->GetRootComponent());
        if (WeaponComponent)
        {
            WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        NearestWeapon->SetActorHiddenInGame(false);
        FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);

        // 무기를 캐릭터에 장착하고, 소켓에 라이트도 연결
        CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("AKGun"));

        UpdateAmmoBar();

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
	float SearchRadius =100.0f;  // 플레이어가 무기를 찾을 범위 (예: 1000 유닛)

	for (AActor* Actor : NearbyWeapons)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(Actor);
		if (Weapon)
		{
			float Distance = FVector::Dist(GetActorLocation(), Weapon->GetActorLocation());

			// 지정된 범위 내에서만 무기를 검색
			if (Distance < SearchRadius && Distance < NearestDistance)
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
		FVector LookAtTarget = WorldLocation + (WorldDirection * 5000.f); 
		FVector CharacterLocation = GetActorLocation();
		FVector Direction = LookAtTarget - CharacterLocation;
		Direction.Z = 0; 
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
	//DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + WorldDirection * 5000.f, FColor::Red, false, 1.0f, 0, 2.0f);
}
void AInfectedCityCharacter::FireBullet()
{
    if (!BulletClass || !CurrentWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet class or weapon is not valid!"));
        return;
    }

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

    FVector CameraLocation;
    FRotator CameraRotation;
    FVector ForwardVector;
    
    // SecondFollowCamera가 활성화되었는지 확인
    if (SecondFollowCamera && SecondFollowCamera->IsActive())
    {
        // SecondFollowCamera의 위치와 회전값을 사용
        CameraLocation = SecondFollowCamera->GetComponentLocation();
        CameraRotation = SecondFollowCamera->GetComponentRotation();
        
        // Second 카메라의 중앙 방향 벡터
        ForwardVector = CameraRotation.Vector();  // 카메라의 앞 방향 벡터
    }
    else
    {
        // 기본 카메라가 사용되면 플레이어 카메라의 위치와 회전값을 사용
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (!PlayerController)
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
            return;
        }
        
        CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        CameraRotation = PlayerController->GetControlRotation();
        
        // 기본 카메라의 중앙 방향 벡터
        ForwardVector = CameraRotation.Vector();  // 카메라의 앞 방향 벡터
    }

    // 총알 발사 위치를 캐릭터의 'Bullet' 소켓으로 설정
    FVector SpawnLocation = GetMesh()->GetSocketLocation("Bullet"); // 총알 출발 위치
    FRotator SpawnRotation = ForwardVector.Rotation(); // 발사 방향 회전 (카메라 방향에 맞춤)

    // 총알을 발사합니다
    if (ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation))
    {
        NewBullet->Fire(ForwardVector, BulletSpeed); // 발사 방향과 속도 설정
    }

    // 발사 애니메이션이 있다면 실행
    if (ShootAnimMontage)
    {
        PlayAnimMontage(ShootAnimMontage);
    }

    // 카메라 회복
    RecoverCameraRecoil();
}
void AInfectedCityCharacter::UpdateAmmoBar()
{
	if (HUDWidget)
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon))
		{
			// 무기의 Ammo 값을 가져와서 HUD에 전달
			float AmmoRatio = Weapon->GetAmmoRatio();  // 0~1 사이의 비율로 Ammo를 표시한다고 가정
			HUDWidget->UpdateAmmoProgress(AmmoRatio);
		}
	}
}

void AInfectedCityCharacter::UpdateReloadText(bool bIsReloading)
{
	if (HUDWidget)
	{
		HUDWidget->SetReloadTextVisibility(bIsReloading);

		if (bIsReloading)
		{
			HUDWidget->PlayReloadAnimation();
		}
	}
}

void AInfectedCityCharacter::AddItem(FName ItemClass, int32 Amount)
{
	// 기존 개수 확인 후 추가
	if (Inventory.Contains(ItemClass))
	{
		Inventory[ItemClass] += Amount;
	}
	else
	{
		Inventory.Add(ItemClass, Amount);
	}
}

void AInfectedCityCharacter::UseItem(FName ItemClass)
{
	if (ItemClass == "Bandage")
	{
		Health += 10;
		/* HP */
		Inventory["Bandage"]--;
	}
	else if (ItemClass == "Pill")
	{
		Stamina += 10;
		/* 스테미나 */
		Inventory["Pill"]--;
	}
	
}

void AInfectedCityCharacter::DrainStamina()
{
	if (Stamina > 0)
	{
		Stamina -= StaminaDrainRate * 0.1f; // 0.1초마다 2씩 감소
		if (HUDWidget)
		{
			HUDWidget->UpdateStaminaBar(Stamina / MaxStamina);
		}

		if (Stamina <= 0)
		{
			Stamina = 0;
			StopRunning();
			bCanRun = false; // 스테미나 0이면 못 달림
		}
	}
}

void AInfectedCityCharacter::RecoverCameraRecoil()
{
	if (!bIsFiring)
	{
		return;  // 총을 쏘지 않으면 함수 종료
	}

	// 카메라 회전 및 위치를 복구하는 함수
	auto RecoverCamera = [this](UCameraComponent* Camera)  // 'this' 캡처
		{
			if (!Camera)
			{
				return;
			}

			// 현재 카메라의 회전과 위치 가져오기
			FRotator CurrentRotation = Camera->GetComponentRotation();
			FVector CurrentLocation = Camera->GetComponentLocation();

			// 반동으로 인한 임의의 회전 (X, Y 회전은 반동 효과 추가)
			FRotator NewRotation = FRotator(
				FMath::RandRange(-RecoilRotationAmount, RecoilRotationAmount), // 임의의 X 회전
				FMath::RandRange(-RecoilRotationAmount, RecoilRotationAmount), // 임의의 Y 회전
				0 // Z 회전은 고정
			);

			// 회전 보정 (부드럽게 회복)
			FRotator RecoilCompensation = FRotator(
				-NewRotation.Pitch, // 반동 회전값을 빼서 보정
				-NewRotation.Yaw,   // 반동 회전값을 빼서 보정
				0                   // Z 회전은 고정
			);

			// 목표 회전 (현재 회전 + 보정값)
			FRotator TargetRotation = CurrentRotation + RecoilCompensation;

			// 회전 부드럽게 회복
			FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RecoilRecoverySpeed);
			Camera->SetWorldRotation(SmoothedRotation);  // 회전 적용

			// 위치 회복 (부드럽게)
			FVector NewLocation = FMath::VInterpTo(CurrentLocation, CurrentLocation + CameraRecoil, GetWorld()->GetDeltaSeconds(), RecoilRecoverySpeed);

			// 위치 보정이 적당히 이루어지도록 카메라 위치를 부드럽게 복구
			Camera->SetWorldLocation(NewLocation);  // 위치 적용
		};

	// FollowCamera와 SecondFollowCamera에 대해 회복 적용
	RecoverCamera(FollowCamera);
	RecoverCamera(SecondFollowCamera);
}


void AInfectedCityCharacter::RecoverStamina()
{
	if (Stamina < MaxStamina)
	{
		Stamina += StaminaRecoveryRate * 0.1f; // 0.1초마다 1씩 회복
		if (HUDWidget)
		{
			HUDWidget->UpdateStaminaBar(Stamina / MaxStamina);
		}

		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
			GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
			bCanRun = true; // 다시 달릴 수 있음
		}
	}
}

void AInfectedCityCharacter::PickupItem()
{
	
	if (ABaseItem* CurrentItem = EnemyEffectManager->GetCurrentItem())
	{
		if (CurrentItem->ItemType == "Bandage")
		{
			BandageCount++;
			HUDWidget->UpdateBandageCount(BandageCount);
			AddItem(TEXT("Bandage"), BandageCount);
		}
		else if (CurrentItem->ItemType == "Pill")
		{
			PillCount++;
			HUDWidget->UpdatePillCount(PillCount);
			AddItem(TEXT("Pill"), PillCount);
		}
		else if (CurrentItem->ItemType == "GasDrum")
		{
			GasCount++;
			HUDWidget->UpdateGasCount(GasCount);
		}
		

		CurrentItem->DestroyItem();
	}
}

float AInfectedCityCharacter::TakeDamage(float DamageAmount)
{
	// 체력 감소
	if (Health > 0)
	{
		bIsPlayingHitAnim = false;
		Health -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage() - 체력 감소: %.2f -> %.2f (데미지: %.2f)"), Health + DamageAmount, Health, DamageAmount);

		// 애니메이션이 진행 중이지 않다면 애니메이션 실행
		
			bIsPlayingHitAnim = true;
			PlayAnimMontage(HitAnimMontage);

			// 애니메이션이 끝났을 때 상태를 다시 원래로 복구하도록 설정
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AInfectedCityCharacter::ResetHitAnimState, HitAnimMontage->GetPlayLength(), false);
		
	}

	UpdateHP(Health);
	// 체력이 0 이하일 때 사망 처리
	if (Health <= 0)
	{
		Health = 0;
		Die(); // 죽음 처리
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage() - 체력이 0 이하로 감소! 사망 처리!"));
	}


	return DamageAmount;  // 데미지가 제대로 적용되었는지 확인하려면 반환값 확인
}

// 애니메이션 상태를 리셋하는 함수
void AInfectedCityCharacter::ResetHitAnimState()
{
	bIsPlayingHitAnim = false;  // 애니메이션이 끝났으므로 상태 리셋
}

void AInfectedCityCharacter::Die()
{
	if (bIsDead)
	{
		// 이미 죽은 상태라면 애니메이션을 다시 실행하지 않음
		return;
	}

	bIsDead = true;  // 캐릭터가 죽었음을 설정

	// 사망 애니메이션 실행
	if (DeathAnimSequence)
	{
		// 애니메이션 재생
		GetMesh()->PlayAnimation(DeathAnimSequence, false);

		// 애니메이션이 끝난 후 캐릭터를 숨기거나 게임 오버 처리를 할 수 있도록 타이머를 설정
		GetWorld()->GetTimerManager().SetTimer(DeathAnimTimerHandle, this, &AInfectedCityCharacter::HandleDeath, DeathAnimSequence->GetPlayLength(), false);
	}
	else
	{
		// DeathAnimSequence가 없다면 그냥 바로 처리
		HandleDeath();
	}

	// 죽은 후 캐릭터의 이동을 막음
	GetCharacterMovement()->DisableMovement();  // 이동 비활성화
	GetCharacterMovement()->StopMovementImmediately();  // 즉시 멈춤
}
void AInfectedCityCharacter::HandleDeath()
{
	// 캐릭터 숨기기 및 충돌 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 게임 오버 처리
	UGameplayStatics::OpenLevel(GetWorld(), "GameOverLevel");
}

void AInfectedCityCharacter::UpdateHP(float NewHP)
{
	CurrentHP = FMath::Clamp(NewHP, 0.0f, MaxHP);

	if (HUDWidget)
	{
		HUDWidget->UpdateHPBar(CurrentHP / MaxHP);
	}
}

void AInfectedCityCharacter::OnRideAvailable()
{
	HUDWidget->StartRideSequence();
}

void AInfectedCityCharacter::DeathEvent()
{
	// 기존 UI 제거
	if (CurrentUIWidget)
	{
		CurrentUIWidget->RemoveFromParent();
		CurrentUIWidget = nullptr;
	}

	// 사망 UI 표시
	if (EndWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			CurrentUIWidget = CreateWidget<UUserWidget>(PC, EndWidgetClass);
			if (CurrentUIWidget)
			{
				CurrentUIWidget->AddToViewport();
			}

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);

			PC->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}

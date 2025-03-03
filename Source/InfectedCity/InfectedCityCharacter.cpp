﻿#include "InfectedCityCharacter.h"
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
#include "PistolWeapon.h"
#include "Components/PointLightComponent.h"


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






}
void AInfectedCityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

<<<<<<< HEAD
	
=======
	// 스포트라이트가 켜져있는지 확인하기 위해 로그 추가
	if (CurrentWeapon)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon);
		if (Weapon && Weapon->Flashlight)
		{
			if (Weapon->Flashlight->IsVisible())
			{
				UE_LOG(LogTemp, Log, TEXT("Spotlight is ON"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Spotlight is OFF"));
			}
		}
	}
>>>>>>> origin/dev
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
		// Pickup Weapon (Mouse Right key)
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AInfectedCityCharacter::StopAiming);
		// Pickup Weapon (Mouse Left key)
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StartShoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AInfectedCityCharacter::StopShoot);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::Reload);

		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::ToggleFlashlight);
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
				
			}

		     
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
		
	}
	
}
void AInfectedCityCharacter::StopShoot()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = false;  //    콺 Ŀ        
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
bool AInfectedCityCharacter::BHASPistol() const
{
	return CurrentWeapon != nullptr && CurrentWeapon->IsA(APistolWeapon::StaticClass());
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

		
		
<<<<<<< HEAD
=======
	}
}

void AInfectedCityCharacter::ToggleFlashlight()
{
	// CurrentWeapon이 유효한지 확인
	if (CurrentWeapon)
	{
		// CurrentWeapon의 플래시라이트를 토글
		CurrentWeapon->ToggleFlashlight();
>>>>>>> origin/dev
	}
}

void AInfectedCityCharacter::ToggleFlashlight()
{
	if (CurrentWeapon)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(CurrentWeapon);
		if (Weapon)
		{
			bool bIsFlashlightOn = Weapon->AreFlashlightsOn();
			Weapon->ToggleFlashlights(!bIsFlashlightOn); // 현재 상태 반대로 토글
		}
	}
}void AInfectedCityCharacter::PickupWeapon()
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
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("AKGun"));



		  
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

   
		if (ShootAnimMontage)
		{
			PlayAnimMontage(ShootAnimMontage);
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
			// 현재 카메라의 회전과 위치 가져오기
			FRotator CurrentRotation = Camera->GetComponentRotation();
			FVector CurrentLocation = Camera->GetComponentLocation();

			// 반동으로 인한 임의의 회전 (X, Y 회전은 반동 효과 추가)
			FRotator NewRotation = FRotator(
				FMath::RandRange(-RecoilRotationAmount, RecoilRotationAmount), // 임의의 X 회전
				FMath::RandRange(-RecoilRotationAmount, RecoilRotationAmount), // 임의의 Y 회전
				0 // Z 회전은 고정
			);

			// 위치 회복 (부드럽게)
			FVector NewLocation = FMath::VInterpTo(CurrentLocation, CurrentLocation + CameraRecoil, GetWorld()->GetDeltaSeconds(), RecoilRecoverySpeed);
			Camera->SetWorldLocation(NewLocation);

			// 회전 보정
			FRotator RecoilCompensation = FRotator(
				-NewRotation.Pitch, // 반동 회전값을 빼서 보정
				-NewRotation.Yaw,   // 반동 회전값을 빼서 보정
				0                   // Z 회전은 고정
			);

			// 회전 부드럽게 회복
			FRotator TargetRotation = CurrentRotation + RecoilCompensation;
			FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RecoilRecoverySpeed);

			// 회전 적용
			Camera->SetWorldRotation(SmoothedRotation);
		};

	// FollowCamera와 SecondFollowCamera에 대해 회복 적용
	RecoverCamera(FollowCamera);
	RecoverCamera(SecondFollowCamera);
}
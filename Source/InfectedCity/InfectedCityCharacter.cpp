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


AInfectedCityCharacter::AInfectedCityCharacter()
{
	
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
		PlayerController->bShowMouseCursor = true;  // 마우스 커서 보이게 설정
		RotateCharacterToMouseCursor();  // 마우스를 따라 캐릭터 회전
	}

	// 기존 카메라에서 새로운 카메라로 전환
	FollowCamera->Deactivate();
	SecondFollowCamera->Activate();

	// 새로운 카메라로 줌 인
	SecondFollowCamera->SetFieldOfView(FMath::FInterpTo(SecondFollowCamera->FieldOfView, ZoomedFOV, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed));

	// 마우스를 따라 회전
	RotateCharacterToMouseCursor();
}

void AInfectedCityCharacter::StopAiming()
{
	bIsAiming = false;

	// 새로운 카메라에서 기존 카메라로 전환
	SecondFollowCamera->Deactivate();
	FollowCamera->Activate();

	// 기존 카메라로 줌 아웃
	FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, DefaultFOV, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = false;  // 마우스 커서 숨기기
	}
}

void AInfectedCityCharacter::StartShoot()
{

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = true;  // 마우스 커서 보이게 설정
		RotateCharacterToMouseCursor();  // 마우스를 따라 캐릭터 회전
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
		PlayerController->bShowMouseCursor = false;  // 마우스 커서 숨기기
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

void AInfectedCityCharacter::PickupWeapon()
{
	AWeaponBase* NearestWeapon = FindNearestWeapon();
	if (NearestWeapon)
	{
		
		CurrentWeapon = NearestWeapon;

		
		UPrimitiveComponent* WeaponComponent = Cast<UPrimitiveComponent>(CurrentWeapon->GetRootComponent());
		if (WeaponComponent)
		{
			// 異⑸룎??鍮꾪솢?깊솕?섏뿬 罹먮┃?곗???異⑸룎???쇳븿
			WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 異⑸룎 鍮꾪솢?깊솕
		}

		// 珥앹씠 蹂댁씠?꾨줉 ?ㅼ젙 (HiddenInGame??false濡??ㅼ젙)
		NearestWeapon->SetActorHiddenInGame(false);

		// 臾닿린瑜??쇱넀 ?뚯폆(AkGun)??遺숈씠湲?
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);  // SnapToTarget: ?뚯폆 ?꾩튂??留욎떠??遺숈엫
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("AKGun"));  // GetMesh()??罹먮┃?곗쓽 Skeletal Mesh 而댄룷?뚰듃

		// 臾닿린 ?쒓굅 (?붾뱶?먯꽌 ?쒓굅?섍굅???몃깽?좊━??異붽??섎뒗 濡쒖쭅 援ы쁽)
		
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
		// 라인 트레이스를 수행하여 마우스가 가리키는 곳을 찾음
		FHitResult HitResult;
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = TraceStart + (WorldDirection * 5000.f); // 5000 유닛 거리까지 검사

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 자기 자신은 무시

		// 라인 트레이스 수행
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			FVector LookAtTarget = HitResult.ImpactPoint;
			FVector CharacterLocation = GetActorLocation();
			FVector Direction = LookAtTarget - CharacterLocation;
			Direction.Z = 0; // 상하 회전 방지

			FRotator NewRotation = Direction.Rotation();
			SetActorRotation(NewRotation);
		}

		// 디버그 라인 (라인 트레이스 확인용)
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
	}
}

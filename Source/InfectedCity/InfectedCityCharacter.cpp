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
#include "InteractManager/InteractManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInfectedCityCharacter

AInfectedCityCharacter::AInfectedCityCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Allow camera to rotate with the player

	// Create the TPS Camera
	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach to camera boom

	// Create the FPS Camera
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(RootComponent); // Attach directly to the player

	// Default to TPS Camera
	bIsTPSCameraActive = true;

	// Set up input actions for camera toggling
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInfectedCityCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Start with the TPS camera
	SwitchToTPSCamera();
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
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

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
		Super::SetupPlayerInputComponent(PlayerInputComponent);


		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::OnRightMouseButtonPressed);

			// V Key (for FPS Camera switch)
		EnhancedInputComponent->BindAction(VKeyAction, ETriggerEvent::Started, this, &AInfectedCityCharacter::OnVKeyPressed);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInfectedCityCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInfectedCityCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Start Running (Shift Key)
void AInfectedCityCharacter::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f; // Increase walk speed for running
}

// Stop Running (Shift Key)
void AInfectedCityCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // Reset walk speed to normal
}

// Start Crouching (Ctrl Key)
void AInfectedCityCharacter::StartCrouching()
{
	Crouch(); // Make the character crouch
}

// Stop Crouching (Ctrl Key)
void AInfectedCityCharacter::StopCrouching()
{
	UnCrouch(); // Make the character stand up
}
void AInfectedCityCharacter::OnRightMouseButtonPressed()
{
	// Switch to TPS Camera (Zoom In or Out)
	if (!bIsTPSCameraActive)
	{
		SwitchToTPSCamera();
	}
}

void AInfectedCityCharacter::OnVKeyPressed()
{
	// Switch to FPS Camera when "V" is pressed
	if (bIsTPSCameraActive)
	{
		SwitchToFPSCamera();
	}
}

void AInfectedCityCharacter::SwitchToTPSCamera()
{
	// Activate the TPS camera and deactivate FPS camera
	TPSCameraComponent->SetActive(true);
	FPSCameraComponent->SetActive(false);
	bIsTPSCameraActive = true;
}

void AInfectedCityCharacter::SwitchToFPSCamera()
{
	// Activate the FPS camera and deactivate TPS camera
	FPSCameraComponent->SetActive(true);
	TPSCameraComponent->SetActive(false);
	bIsTPSCameraActive = false;
}
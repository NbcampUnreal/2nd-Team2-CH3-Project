#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "OutlineComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionObjectType(ECC_GameTraceChannel2);
    Collision->SetupAttachment(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);

    OutlineComponent = CreateDefaultSubobject<UOutlineComponent>(TEXT("OutlineComponent"));

    PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
    PickupWidget->SetupAttachment(RootComponent);
    PickupWidget->SetWidgetSpace(EWidgetSpace::World);
    PickupWidget->SetDrawSize(FVector2D(18, 31));
    PickupWidget->SetVisibility(false);
    
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/Item/WBP_Pickup.WBP_Pickup"));
    if (WidgetClassFinder.Succeeded())
    {
        PickupWidget->SetWidgetClass(WidgetClassFinder.Class);
    }
}

void AItemBase::OnItem()
{
    OutlineComponent->EnableOutline();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        FRotator LookAtRotation = (CameraLocation - PickupWidget->GetComponentLocation()).Rotation();

        PickupWidget->SetWorldRotation(LookAtRotation);
    }

    PickupWidget->SetVisibility(true);

    if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
    {
    //    UE_LOG(LogTemp, Warning, TEXT("Active %s"), *GetOwner()->GetName());
        ActivateItem();
    }
}

void AItemBase::OnItemEnd()
{
    OutlineComponent->DisableOutline();
    PickupWidget->SetVisibility(false);
}

void AItemBase::ActivateItem()
{
    DestroyItem();
}

FName AItemBase::GetItemType()
{
	return ItemType;
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PickupWidget->IsWidgetVisible() == false)
        return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        FRotator LookAtRotation = (CameraLocation - PickupWidget->GetComponentLocation()).Rotation();

        PickupWidget->SetWorldRotation(LookAtRotation);
    }


}

void AItemBase::DestroyItem()
{
    UE_LOG(LogTemp, Warning, TEXT("Destroy %s"), *GetOwner()->GetName());
    Destroy();
}


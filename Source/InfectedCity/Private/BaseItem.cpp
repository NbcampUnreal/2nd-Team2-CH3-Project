#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "OutlineComponent.h"

ABaseItem::ABaseItem()
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

void ABaseItem::OnItem()
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

void ABaseItem::OnItemEnd()
{
    OutlineComponent->DisableOutline();
    PickupWidget->SetVisibility(false);
}

void ABaseItem::ActivateItem()
{
    DestroyItem();
}

FName ABaseItem::GetItemType()
{
    return ItemType;
}

void ABaseItem::Tick(float DeltaTime)
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

void ABaseItem::UseItem_Implementation(AActor* User)
{

}

void ABaseItem::DestroyItem()
{
    Destroy();
}



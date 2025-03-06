#include "Station.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"


AStation::AStation()
{
    PrimaryActorTick.bCanEverTick = true;

    // Scene Root ���� (�ڽ��� �ؽ�Ʈ�� �׷�ȭ)
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // �ڽ� �ݶ��̴� ����
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(SceneRoot);
    BoxComponent->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStation::OnOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStation::OnOverlapEnd);

    // �ؽ�Ʈ ������Ʈ (���� ���� ��� ����)
    TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
    TextComponent->SetupAttachment(SceneRoot);
    TextComponent->SetText(FText::FromString(TEXT("0")));
    TextComponent->SetWorldSize(100.0f);
    TextComponent->SetHorizontalAlignment(EHTA_Center);
    TextComponent->SetVerticalAlignment(EVRTA_TextCenter);
    TextComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f)); // ���� ���� ��ġ
}

void AStation::BeginPlay()
{
    Super::BeginPlay();
}

void AStation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OverlappingPawn)
    {
        APlayerController* PlayerController = Cast<APlayerController>(OverlappingPawn->GetController());
        if (PlayerController)
        {
            if (PlayerController->WasInputKeyJustPressed(EKeys::F))
            {
                UE_LOG(LogTemp, Log, TEXT("F key pressed"));
                HandleInput();
            }
            else
            {
            }
        }
    }
}


void AStation::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    OverlappingPawn = Cast<AInfectedCityCharacter>(OtherActor);
}

void AStation::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OverlappingPawn == OtherActor)
    {
        OverlappingPawn = nullptr;
    }
}

void AStation::HandleInput()
{
    AInfectedCityCharacter* PlayerCharacter = Cast<AInfectedCityCharacter>(OverlappingPawn);
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("Current GasCount: %d"), PlayerCharacter->GasCount);
        if (PlayerCharacter->GasCount > 0)
        {
            PlayerCharacter->GasCount--;
            StationCount++;
            TextComponent->SetText(FText::AsNumber(StationCount));

            if (StationCount >= 5)
            {
                CanOut = true;
            }

            if (PlayerCharacter->HUDWidget)
            {
                UHUDWidget* HUDWidget = Cast<UHUDWidget>(PlayerCharacter->HUDWidget);
                if (HUDWidget)
                {
                    HUDWidget->UpdateGasCount(PlayerCharacter->GasCount);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("HUDWidget is nullptr in HandleInput!"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GasCount is 0 or less. No action performed."));
        }
    }
}

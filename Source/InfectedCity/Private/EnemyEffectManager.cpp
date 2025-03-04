#include "EnemyEffectManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include <Camera/CameraComponent.h>
#include <Kismet/KismetRenderingLibrary.h>
#include "ItemBase.h"


AEnemyEffectManager::AEnemyEffectManager()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    RootComponent = SceneCapture;
}

void AEnemyEffectManager::BeginPlay()
{
	Super::BeginPlay();
    //SetupSceneCapture();

    
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        bPreMouseCursor = PlayerController->bShowMouseCursor;
    }
}

void AEnemyEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    


    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        /* Cursor Test Code */
        if (PlayerController->IsInputKeyDown(EKeys::F1))
        {
            UE_LOG(LogTemp, Log, TEXT("Shift + F1 Down"));
            PlayerController->bShowMouseCursor = !PlayerController->bShowMouseCursor;
        }

        if (bPreMouseCursor != PlayerController->bShowMouseCursor)
        {
            if (EnemyCharacter)
            {
                EnemyCharacter->EnableOutline(false);
                EnemyCharacter = nullptr;
            }

            bPreMouseCursor = PlayerController->bShowMouseCursor;
        }

        PlayerController->bShowMouseCursor == true ? DetectActorAtMouseCursor(PlayerController) : DetectActorAtCenter(PlayerController);
    }

    CheckPlayerRaytrace();
}

void AEnemyEffectManager::SetupSceneCapture()
{
    if (!GetWorld() || !PostProcessMaterial) return;

    /* Setup SceneCapture */
    DepthRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    DepthRenderTarget->RenderTargetFormat = RTF_R32f;
    DepthRenderTarget->InitAutoFormat(1920, 1080);
    DepthRenderTarget->UpdateResourceImmediate();

    SceneCapture->TextureTarget = DepthRenderTarget;
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;

    /* Setup Enemy Binding */
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundEnemies);

    SceneCapture->ShowOnlyActors.Empty();

    for (AActor* Enemy : FoundEnemies)
    {
        SceneCapture->ShowOnlyActors.Add(Enemy);
    }

    /* Setup Material Binding */
    APostProcessVolume* PPVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
    if (!PPVolume)
    {
        UE_LOG(LogTemp, Warning, TEXT("Post Process Volume을 찾을 수 없음"));
        return;
    }

    if (UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(PostProcessMaterial, this))
    {
        DynamicMat->SetTextureParameterValue(FName("DepthRT"), DepthRenderTarget);
        PPVolume->Settings.AddBlendable(DynamicMat, 1.0f);
    }

    ///* Attach Player Camera */
    //if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    //{
    //    if (APawn* PlayerPawn = PlayerController->GetPawn())
    //    {
    //        if (UCameraComponent* PlayerCamera = PlayerPawn->FindComponentByClass<UCameraComponent>())
    //        {
    //            AttachToComponent(PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
    //        }
    //    }
    //}
}

void AEnemyEffectManager::DetectActorAtMouseCursor(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        FVector2D MousePosition;
        PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

        FVector WorldLocation;
        FVector WorldDirection;
        PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(PlayerController->GetPawn());

        FVector Start = WorldLocation;
        FVector End = WorldLocation + (WorldDirection * 10000.0f);

        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByObjectType(
            HitResult,
            Start,
            End,
            ECC_GameTraceChannel1,
            TraceParams
        );

        if (bHit)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("Actor Mouse hit: %s"), *HitActor->GetName());

                if (!EnemyCharacter)
                {
                    EnemyCharacter = Cast<AEnemyCharacter>(HitActor);
                    EnemyCharacter->EnableOutline(true);
                }

            }
        }
        else
        {
            if (EnemyCharacter)
            {
                EnemyCharacter->EnableOutline(false);
            }
            EnemyCharacter = nullptr;
        }
    }
}

void AEnemyEffectManager::DetectActorAtCenter(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        FVector2D ViewportSize;
        GEngine->GameViewport->GetViewportSize(ViewportSize);

        FVector2D ScreenCenter = ViewportSize * 0.5f;

        FVector WorldLocation;
        FVector WorldDirection;
        PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(PlayerController->GetPawn());

        FVector Start = WorldLocation;
        FVector End = WorldLocation + (WorldDirection * 10000.0f);

        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByObjectType(
            HitResult,
            Start,
            End,
            ECC_GameTraceChannel1,
            TraceParams
        );

        if (bHit)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("Actor Center hit: %s"), *HitActor->GetName());

                if (!EnemyCharacter)
                {
                    EnemyCharacter = Cast<AEnemyCharacter>(HitActor);
                    EnemyCharacter->EnableOutline(true);
                }

            }
        }
        else
        {
            if (EnemyCharacter)
            {
                EnemyCharacter->EnableOutline(false);
            }
            EnemyCharacter = nullptr;
        }
    }
}

void AEnemyEffectManager::CheckPlayerRaytrace()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    FVector2D MousePosition;
    PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

    FVector WorldLocation;
    FVector WorldDirection;
    PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(PlayerController->GetPawn());

    FVector Start = WorldLocation;
    FVector End = WorldLocation + (WorldDirection * 10000.0f);

    FHitResult HitResult;
    bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ECC_GameTraceChannel2, TraceParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor Center hit: %s"), *HitActor->GetName());

            if (!CurrentItem)
            {
                CurrentItem = Cast<AItemBase>(HitActor);
                CurrentItem->OnItem();
            }

        }
    }
    else
    {
        if (CurrentItem)
        {
            CurrentItem->OnItemEnd();
        }
        CurrentItem = nullptr;
    }
}
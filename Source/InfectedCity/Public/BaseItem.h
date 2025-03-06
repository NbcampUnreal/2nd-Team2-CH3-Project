// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class USphereComponent;
class UOutlineComponent;
class UWidgetComponent;

UCLASS()
class INFECTEDCITY_API ABaseItem : public AActor, public IItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemType;
protected: /*ktw*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effects")
    UOutlineComponent* OutlineComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetComponent* PickupWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* PickupParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* PickupSound;

public: /*ktw*/
    virtual void OnItem();
    virtual void OnItemEnd();
    virtual void ActivateItem();
    virtual FName GetItemType();

    void DestroyItem();

    virtual void Tick(float DeltaTime) override;

    virtual void UseItem(AActor* User);
};

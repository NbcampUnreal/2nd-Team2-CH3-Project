#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UOutlineComponent;
class UWidgetComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UItemBase : public UInterface
{
    GENERATED_BODY()
};

class INFECTEDCITY_API IItemBase
{
    GENERATED_BODY()

protected:
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

    virtual void Tick(float DeltaTime) override;

    void DestroyItem();
public:
    // ������ ��� �Լ� (��ӹ��� Ŭ�������� ����)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void UseItem(AActor* User);
};

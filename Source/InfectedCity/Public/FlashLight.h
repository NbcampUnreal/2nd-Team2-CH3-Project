// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "FlashLight.generated.h"

UCLASS()
class INFECTEDCITY_API AFlashLight : public AActor
{
	GENERATED_BODY()
	
public:
    AFlashLight();

    // �÷��ö���Ʈ�� �޽� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
    UStaticMeshComponent* FlashlightMesh;

    // �÷��ö���Ʈ�� �浹 �ڽ� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionSphere;

    // �÷��ö���Ʈ�� �޽� ������Ʈ�� ��ȯ�ϴ� �Լ�
    UStaticMeshComponent* GetMeshComponent() const { return FlashlightMesh; }

};
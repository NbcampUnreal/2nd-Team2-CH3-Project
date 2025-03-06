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

    // 플래시라이트의 메쉬 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
    UStaticMeshComponent* FlashlightMesh;

    // 플래시라이트의 충돌 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionSphere;

    // 플래시라이트의 메쉬 컴포넌트를 반환하는 함수
    UStaticMeshComponent* GetMeshComponent() const { return FlashlightMesh; }

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GN_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTEDCITY_API UGN_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    // ���� �̵� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Speed;

    // �޸��� ������ Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsRunning;

    // ���� ������ Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsAttacking;

    // ���� ���� Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsDead;

    // ���� ������Ʈ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void UpdateAnimationProperties();
};

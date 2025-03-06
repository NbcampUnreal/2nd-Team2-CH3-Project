// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GN_ChaseMan.generated.h"

UCLASS()
class INFECTEDCITY_API AGN_ChaseMan : public ACharacter
{
    GENERATED_BODY()

public:
    AGN_ChaseMan();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    void ChasePlayer();
    void AttackPlayer();
    void CheckAttackRange();
    void Dead();
    void AttackEnd();
    void TakeDamage(float DamageAmount);

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float ChaseSpeed = 600.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float AttackRange = 100.0f;

    APawn* TargetPlayer;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimationAsset* IdleAnimation;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimationAsset* WalkAnimation;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimationAsset* RunAnimation;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimationAsset* AttackAnimation;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimationAsset* DeathAnimation;

    float MaxHealth = 100.0f;

    float CurrentHealth;

    UAnimationAsset* PreAnimation;
    UAnimationAsset* CurAnimation;

    FTimerHandle DeadAnimTimerHandle;
    FTimerHandle AttackTimerHandle;

    int32 Random;

    bool bIsAttacking;

    USkeletalMeshComponent* MeshComp;
};

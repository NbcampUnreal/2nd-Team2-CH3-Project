#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimMontage.h"
#include "GN_Character.generated.h"

class UOutlineComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
    Idle,
    Patrolling,
    Chasing,
    Attacking,
    Dead
};

UCLASS()
class INFECTEDCITY_API AGN_Character : public ACharacter {
    GENERATED_BODY()

public:
    AGN_Character();

    //UPROPERTY(EditAnywhere, Category = "Animations")
    //class UGN_AnimInstance* AnimInstance;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // AI             Pawn Sensing
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UPawnSensingComponent* PawnSensingComp;

    // AI     
    EEnemyState CurrentState;
    void SetEnemyState(EEnemyState NewState);

    //  ??  ? 
    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* AttackAnimation;

    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* DeathAnimation;

    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* IdleAnimation;

    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* WalkAnimation;

    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* RunAnimation;

    UPROPERTY(EditAnywhere, Category = "Animations")
    UAnimationAsset* ScreamAnimation;


    //체력 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentHealth;

    void TakeDamage(float DamageAmount);

    void PlayAttackAnimation();
    void PlayDeathAnimation();

    // AI  ?
    void Patrol();
    void ChasePlayer(APawn* Player);
    void Attack();
    void Die();
    void StartChase();
    void CheckIfPlayerLost();

    void Dead();
    void AttackEnd();
    class AGN_AIController* AIController;

private:
    UAnimationAsset* PreAnimation{ nullptr };
    UAnimationAsset* CurAnimation{ nullptr };

    FTimerHandle ScreamTimerHandle;
    FTimerHandle PatrolTimerHandle;
    FTimerHandle DeadAnimTimerHandle;
    float AttackRange = 150.0f;

    int32 Random;

    USkeletalMeshComponent* MeshComp;

    APawn* CurTarget{ nullptr };
    UFUNCTION()
    void OnSeePawn(APawn* Pawn);
public:
    void EnableOutline(bool bEnable);
private:
    UPROPERTY(VisibleAnywhere, Category = "Effects")
    UOutlineComponent* OutlineComponent;
};

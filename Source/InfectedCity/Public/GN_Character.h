#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimMontage.h"
#include "GN_Character.generated.h"

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

    UPROPERTY(EditAnywhere, Category = "Animations")
    class UGN_AnimInstance* AnimInstance;


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // AI ������ ���� Pawn Sensing
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UPawnSensingComponent* PawnSensingComp;

    // AI ����
    EEnemyState CurrentState;
    void SetEnemyState(EEnemyState NewState);

    // �ִϸ��̼�
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


    void PlayAttackAnimation();
    void PlayDeathAnimation();

    // AI �ൿ
    void Patrol();
    void ChasePlayer(APawn* Player);
    void Attack();
    void Die();

    class AGN_AIController* AIController;

private:
    UAnimationAsset* PreAnimation{ nullptr };
    UAnimationAsset* CurAnimation{ nullptr };

    APawn* CurTarget{ nullptr };
    UFUNCTION()
    void OnSeePawn(APawn* Pawn);

};

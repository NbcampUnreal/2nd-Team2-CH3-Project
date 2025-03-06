#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BillboardComponent.h"
#include "GN_Character.generated.h"

class UBillboardComponent;
class UBoxComponent;
class AInfectedCityCharacter;
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
    UAnimationAsset* ScreamAnimation;  // 비명 애니메이션 추가

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards")
    UBillboardComponent* RightArmStart;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards")
    UBillboardComponent* RightArmEnd;


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

    APawn* CurTarget{ nullptr };
    UFUNCTION()
    void OnSeePawn(APawn* Pawn);

public:
    void EnableOutline(bool bEnable);
   
    // 오른팔 시작과 끝에 충돌을 처리할 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* RightArmStartCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* RightArmEndCollider;

    // 충돌이 시작될 때 호출되는 함수
    UFUNCTION()
    void OnRightArmStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
                                bool bFromSweep, const FHitResult& SweepResult);


   

    UFUNCTION()
    void EnableRightArmCollision();

    UFUNCTION()
    void DisableRightArmCollision();
    // 플레이어에게 데미지를 적용하는 함수
    void ApplyDamageToPlayer(AActor* Player);
    virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
    virtual void OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
private:
    UPROPERTY(VisibleAnywhere, Category = "Effects")
    UOutlineComponent* OutlineComponent;

};

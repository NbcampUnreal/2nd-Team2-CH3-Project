﻿#include "GN_Character.h"
#include "GN_AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GN_AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

AGN_Character::AGN_Character()
{
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AGN_Character::OnSeePawn);

    // 감지 반경 및 시야각 설정
    PawnSensingComp->SightRadius = 1500.0f;  // 플레이어 감지 거리 (기존보다 넓힘)
    PawnSensingComp->SetPeripheralVisionAngle(100.0f);  // 시야각 설정 (기존보다 넓힘)
    PawnSensingComp->SensingInterval = 0.25f;  // 감지 주기 조정 (더 빠르게 감지)

    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    CurrentState = EEnemyState::Idle;

    // 체력 초기화
    CurrentHealth = MaxHealth;
}

void AGN_Character::BeginPlay()
{
    Super::BeginPlay();

   /* AnimInstance = Cast<UAGN_AnimInstance>(GetMesh()->GetAnimInstance());*/
    AIController = Cast<AGN_AIController>(GetController());

    PreAnimation = IdleAnimation;
    CurAnimation = IdleAnimation;
    Patrol();
}

void AGN_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float Speed = GetVelocity().Size();

    if (Speed == 0)
    {
        PreAnimation = IdleAnimation;
    }
    else if (Speed > 0 && Speed < 400)
    {
        PreAnimation = WalkAnimation;
    }
    else if (Speed >= 400)
    {
        PreAnimation = RunAnimation;
    }

    if (CurAnimation != PreAnimation)
    {
        GetMesh()->PlayAnimation(PreAnimation, true);
        CurAnimation = PreAnimation;
    }

    if (CurrentState == EEnemyState::Chasing)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
        if (Player)
        {
            float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
            UE_LOG(LogTemp, Warning, TEXT("추적 중... 남은 거리: %f"), Distance);

            // 공격 범위 내에 들어오면 공격 실행
            if (Distance <= AttackRange)
            {
                UE_LOG(LogTemp, Warning, TEXT("공격 범위에 도달! 공격 시작!"));
                Attack();
                return;  // 공격 후 이동을 중단
            }

            // 플레이어를 놓친 경우 순찰로 복귀
            if (Distance > 2000.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("플레이어가 범위를 벗어남. 3초 후 다시 확인."));
                GetWorld()->GetTimerManager().SetTimer(
                    PatrolTimerHandle,
                    this,
                    &AGN_Character::CheckIfPlayerLost,
                    3.0f,
                    false
                );
            }
            else
            {
                // 계속해서 추적하도록 MoveToActor 반복 호출
                if (AIController)
                {
                    AIController->MoveToActor(Player);
                }
            }
        }
    }
}


void AGN_Character::OnSeePawn(APawn* Pawn)
{
    if (CurrentState == EEnemyState::Dead)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnSeePawn() - AI가 이미 죽어 있음. 감지 무시."));
        return;
    }

    if (CurrentState == EEnemyState::Chasing)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnSeePawn() - AI가 이미 플레이어를 쫓고 있음. 감지 무시."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("OnSeePawn() - 플레이어를 감지하고 추적 시작"));

    CurTarget = Pawn;

    SetEnemyState(EEnemyState::Chasing);

    if (AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnSeePawn() - AIController가 존재함. MoveToActor() 호출"));
        AIController->MoveToActor(CurTarget);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnSeePawn() - AIController가 NULL임! AI 이동 불가!"));
    }
}


void AGN_Character::SetEnemyState(EEnemyState NewState)
{
    if (CurrentState == NewState)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetEnemyState() - 이미 상태가 %d 입니다. 변경하지 않음."), (int)NewState);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("SetEnemyState() - 상태 변경: %d -> %d"), (int)CurrentState, (int)NewState);

    CurrentState = NewState;

    if (NewState == EEnemyState::Chasing)
    {
        GetCharacterMovement()->MaxWalkSpeed = 800.0f;
        GetCharacterMovement()->MaxAcceleration = 1500.0f;
        UE_LOG(LogTemp, Warning, TEXT("SetEnemyState() - Chasing 상태로 이동 속도 설정: %f"), GetCharacterMovement()->MaxWalkSpeed);
    }
    else if (NewState == EEnemyState::Attacking)
    {
        GetCharacterMovement()->MaxWalkSpeed = 0.0f;
        PlayAttackAnimation();
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    }

    if (NewState == EEnemyState::Dead)
    {
        PlayDeathAnimation();
    }
}


void AGN_Character::Patrol()
{
    if (AIController)
    {
        AIController->StartPatrolling();
        SetEnemyState(EEnemyState::Patrolling);
    }
}

void AGN_Character::ChasePlayer(APawn* Player)
{
    if (AIController && IsValid(Player))
    {
        if (CurTarget != Player)  // 같은 목표를 다시 설정하지 않음
        {
            UE_LOG(LogTemp, Warning, TEXT("ChasePlayer() - 플레이어 추적 시작!"));
            CurTarget = Player;
            SetEnemyState(EEnemyState::Chasing);
            AIController->MoveToActor(Player);
        }
    }
}

void AGN_Character::Attack()
{
    if (CurrentState == EEnemyState::Attacking || CurrentState == EEnemyState::Dead)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack() - 이미 공격 중이거나 사망 상태임. 공격 취소."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Attack() - 공격 시작!"));

    SetEnemyState(EEnemyState::Attacking);
    PlayAttackAnimation();

    GetWorld()->GetTimerManager().SetTimer(
        PatrolTimerHandle,
        this,
        &AGN_Character::StartChase,  // 1.5초 후 다시 추격 시작
        1.5f,
        false
    );
}

void AGN_Character::Die()
{
    SetEnemyState(EEnemyState::Dead);
    PlayDeathAnimation();
}

void AGN_Character::PlayAttackAnimation()
{
    if (AttackAnimation)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayAttackAnimation() - 공격 애니메이션 실행!"));
        GetMesh()->PlayAnimation(AttackAnimation, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayAttackAnimation() - AttackAnimation이 설정되지 않음!"));
    }
}

void AGN_Character::PlayDeathAnimation()
{
    if (DeathAnimation)
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);
    }
}

//비명
void AGN_Character::StartChase()
{
    if (CurTarget)
    {
        SetEnemyState(EEnemyState::Chasing);
        AIController->MoveToActor(CurTarget);
    }
}

//체력 감소 및 죽음 처리
void AGN_Character::TakeDamage(float DamageAmount)
{
    if (CurrentHealth <= 0.0f || CurrentState == EEnemyState::Dead)
        return;

    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }
}

void AGN_Character::CheckIfPlayerLost()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player || FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 2000.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어를 완전히 놓침. 순찰 시작."));
        Patrol();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어가 아직 감지됨. 추적 유지."));
        AIController->MoveToActor(Player);
    }
}

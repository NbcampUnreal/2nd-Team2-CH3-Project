#include "GN_Character.h"
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

    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
    CurrentState = EEnemyState::Idle;
}

void AGN_Character::BeginPlay()
{
    Super::BeginPlay();
    
    AnimInstance = Cast<UGN_AnimInstance>(GetMesh()->GetAnimInstance());
    AIController = Cast<AGN_AIController>(GetController());
    Patrol();
}

void AGN_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���� �̵� �ӵ� ��������
    float Speed = GetVelocity().Size();

    // �ִϸ��̼� ���� ����
    if (Speed == 0)
    {
        // Idle �ִϸ��̼� ����
        if (IdleAnimation)
        {
            GetMesh()->PlayAnimation(IdleAnimation, true);
        }
    }
    else if (Speed > 0 && Speed < 400)
    {
        // �ȱ� �ִϸ��̼� ����
        if (WalkAnimation)
        {
            GetMesh()->PlayAnimation(WalkAnimation, true);
        }
    }
    else if (Speed >= 400)
    {
        // �ٱ� �ִϸ��̼� ����
        if (RunAnimation)
        {
            GetMesh()->PlayAnimation(RunAnimation, true);
        }
    }

    // ������ AI ���� ���� �ڵ� ���� (�÷��̾ ��ġ�� �ٽ� ����)
    if (CurrentState == EEnemyState::Chasing)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
        if (Player && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 2000.0f)
        {
            Patrol();
        }
    }
}


void AGN_Character::OnSeePawn(APawn* Pawn)
{
    if (CurrentState != EEnemyState::Dead)
    {
        ChasePlayer(Pawn);
    }
}

void AGN_Character::SetEnemyState(EEnemyState NewState)
{
    CurrentState = NewState;

    if (NewState == EEnemyState::Chasing)
    {
        GetCharacterMovement()->MaxWalkSpeed = 600.0f; // �ٱ� �ӵ�
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = 200.0f; // �ȱ� �ӵ�
    }

    if (NewState == EEnemyState::Dead)
    {
        PlayDeathAnimation();
    }
    else if (NewState == EEnemyState::Attacking)
    {
        PlayAttackAnimation();
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
    if (AIController)
    {
        AIController->MoveToActor(Player);
        SetEnemyState(EEnemyState::Chasing);
    }
}

void AGN_Character::Attack()
{
    SetEnemyState(EEnemyState::Attacking);
    PlayAttackAnimation();
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
        GetMesh()->PlayAnimation(AttackAnimation, false);
    }
}

void AGN_Character::PlayDeathAnimation()
{
    if (DeathAnimation)
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);
    }
}

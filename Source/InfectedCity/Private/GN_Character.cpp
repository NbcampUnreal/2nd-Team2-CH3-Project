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
        UE_LOG(LogTemp, Warning, TEXT("Seen Pawn: %s"), *Pawn->GetName());

        AIController->MoveToActor(Cast<AActor>(Pawn));
        
        SetEnemyState(EEnemyState::Chasing);
    }
}

void AGN_Character::SetEnemyState(EEnemyState NewState)
{
    if (CurrentState == NewState)
        return;

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
    if (AIController && IsValid(Player))
    {
        if (CurTarget != Player)
        {
            AIController->MoveToActor(Player);
            CurTarget = Player;
            SetEnemyState(EEnemyState::Chasing);
        }
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

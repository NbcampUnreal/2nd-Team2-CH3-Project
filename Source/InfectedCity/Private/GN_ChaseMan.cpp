#include "GN_ChaseMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/Engine.h"

AGN_ChaseMan::AGN_ChaseMan()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;

    CurrentHealth = MaxHealth;
}

void AGN_ChaseMan::BeginPlay()
{
    Super::BeginPlay();

    MeshComp = GetMesh();

    if (!MeshComp) return;

    const TCHAR* MeshPaths[] = {
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Ch10_nonPBR.Ch10_nonPBR'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Girlscout_T_Masuyama.Girlscout_T_Masuyama'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Parasite_L_Starkie.Parasite_L_Starkie'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Pumpkinhulk_L_Shaw.Pumpkinhulk_L_Shaw'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Warzombie_F_Pedroso.Warzombie_F_Pedroso'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Yaku_J_Ignite.Yaku_J_Ignite'"),
        TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Zombie/Mesh/Zombiegirl_W_Kurniawan.Zombiegirl_W_Kurniawan'")
    };
             
    Random = FMath::RandRange(0, 6);

    USkeletalMesh* RandomMesh = LoadObject<USkeletalMesh>(nullptr, MeshPaths[Random]);

    if (RandomMesh)
    {
        MeshComp->SetSkeletalMesh(RandomMesh);
    }

    TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);

    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

    PreAnimation = IdleAnimation;
    CurAnimation = IdleAnimation;

    if (IdleAnimation)
    {
        GetMesh()->PlayAnimation(IdleAnimation, true);
    }
}

void AGN_ChaseMan::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsAttacking)
    {
        ChasePlayer();
    }

    CheckAttackRange();
}

void AGN_ChaseMan::ChasePlayer()
{
    if (TargetPlayer && !bIsAttacking)
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->MoveToActor(TargetPlayer);

            if (CurAnimation != RunAnimation)
            {
                CurAnimation = RunAnimation;
                GetMesh()->PlayAnimation(CurAnimation, true);
            }
        }
    }
}

void AGN_ChaseMan::CheckAttackRange()
{
    if (TargetPlayer && FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation()) <= AttackRange)
    {
        AttackPlayer();
    }
}

void AGN_ChaseMan::AttackPlayer()
{
    if (bIsAttacking || !AttackAnimation)
    {
        return;
    }

    bIsAttacking = true;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
    }
    GetCharacterMovement()->StopMovementImmediately();

    GetMesh()->PlayAnimation(AttackAnimation, false);

    float AnimDuration = AttackAnimation->GetPlayLength();
    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &AGN_ChaseMan::AttackEnd,
        AnimDuration,
        false
    );
}

void AGN_ChaseMan::AttackEnd()
{
    bIsAttacking = false;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController && TargetPlayer)
    {
        AIController->MoveToActor(TargetPlayer);
    }

    if (RunAnimation)
    {
        CurAnimation = RunAnimation;
        GetMesh()->PlayAnimation(CurAnimation, true);
    }
}

void AGN_ChaseMan::TakeDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.0f)
    {
        Dead();
    }
}

void AGN_ChaseMan::Dead()
{
    if (DeathAnimation)
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);
    }
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);
}

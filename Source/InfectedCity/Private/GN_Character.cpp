#include "GN_Character.h"
#include "GN_AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GN_AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "OutlineComponent.h"
#include "InfectedCity/InfectedCityCharacter.h"
#include "Components/BoxComponent.h"


AGN_Character::AGN_Character()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

    OutlineComponent = CreateDefaultSubobject<UOutlineComponent>(TEXT("OutlineComponent"));

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

    

    // 충돌 박스 생성
    RightArmStartCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArmStartCollider"));
    RightArmStartCollider->SetupAttachment(GetMesh(), FName("RightHand"));  // RightHand 소켓에 첨부
    

    // 충돌 발생 시 처리할 함수 설정
    RightArmStartCollider->OnComponentBeginOverlap.AddDynamic(this, &AGN_Character::OnRightArmStartOverlap);
    RightArmStartCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RightArmStartCollider->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본값을 모든 채널에서 무시
    RightArmStartCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // 캐릭터와의 충돌은 Overlap


}

void AGN_Character::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (NotifyName == "Attack")
    {
        
        EnableRightArmCollision();
        UE_LOG(LogTemp, Warning, TEXT("NotifyBegin - 공격 충돌 활성화!"));
    }
}

void AGN_Character::OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (NotifyName == "Attack")
    {
        
        DisableRightArmCollision();
        UE_LOG(LogTemp, Warning, TEXT("NotifyEnd - 공격 충돌 비활성화!"));
    }
}


void AGN_Character::EnableRightArmCollision()
{
    if (RightArmStartCollider)
    {
        RightArmStartCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 충돌 활성화
        RightArmStartCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 
        RightArmStartCollider->SetSimulatePhysics(true);  // 물리적 시뮬레이션 활성화// Pawn(플레이어)와의 충돌을 오버랩으로 설정
        UE_LOG(LogTemp, Warning, TEXT("RightArmStartCollider 충돌 활성화"));
    }
}

void AGN_Character::DisableRightArmCollision()
{
    if (RightArmStartCollider)
    {
        RightArmStartCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
        RightArmStartCollider->SetSimulatePhysics(false);
        UE_LOG(LogTemp, Warning, TEXT("RightArmStartCollider 충돌 비활성화"));
    }
}
void AGN_Character::OnRightArmStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this) // 자신과의 충돌이 아닌 경우
    {
        
        if (CurrentState == EEnemyState::Attacking) 
        {
            AInfectedCityCharacter* Player = Cast<AInfectedCityCharacter>(OtherActor);
            if (Player)
            {
                // 플레이어가 충돌 시작 시 데미지 처리
                UE_LOG(LogTemp, Warning, TEXT("플레이어가 RightArmStart에 충돌했습니다."));
                ApplyDamageToPlayer(OtherActor);
            }

        }
        
    }
}
// 데미지 적용 함수
void AGN_Character::ApplyDamageToPlayer(AActor* Player)
{
    if (Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어가 충돌하여 데미지를 받았습니다."));

        // 데미지 처리 예시 (플레이어가 AInfectedCityCharacter인 경우)
        AInfectedCityCharacter* InfectedCityCharacter = Cast<AInfectedCityCharacter>(Player);
        if (InfectedCityCharacter)
        {
            InfectedCityCharacter->TakeDamage(10.0f); // 데미지 10 적용
        }
    }
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

    if (CurrentState != EEnemyState::Dead && CurrentState != EEnemyState::Attacking)
    {
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
    }

    if (CurAnimation != PreAnimation)
    {
        if (PreAnimation == DeathAnimation)
        {
            float AnimDuration = PreAnimation->GetPlayLength();

            GetMesh()->PlayAnimation(PreAnimation, false);
            
            GetWorldTimerManager().SetTimer(DeadAnimTimerHandle, this, &AGN_Character::Dead, AnimDuration, false);
        }
        else if (PreAnimation == AttackAnimation)
        {
            float AnimDuration = PreAnimation->GetPlayLength();
            GetMesh()->PlayAnimation(PreAnimation, false);  // 어택 애니메이션 실행
            GetWorldTimerManager().SetTimer(DeadAnimTimerHandle, this, &AGN_Character::AttackEnd, AnimDuration, false);
        }
        else
        {
            GetMesh()->PlayAnimation(PreAnimation, true);
        }
        CurAnimation = PreAnimation;
    }

    if (CurrentState == EEnemyState::Chasing)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
        if (Player)
        {
            float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
            //UE_LOG(LogTemp, Warning, TEXT("추적 중... 남은 거리: %f"), Distance);

            // 공격 범위 내에 들어오면 공격 실행
            if (Distance <= AttackRange)
            {
                //UE_LOG(LogTemp, Warning, TEXT("공격 범위에 도달! 공격 시작!"));
                Attack();
                return;  // 공격 후 이동을 중단
            }

            // 플레이어를 놓친 경우 순찰로 복귀
            if (Distance > 2000.0f)
            {
                //UE_LOG(LogTemp, Warning, TEXT("플레이어가 범위를 벗어남. 3초 후 다시 확인."));
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

    if (CurrentState == EEnemyState::Chasing || CurrentState == EEnemyState::Attacking)
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

void AGN_Character::EnableOutline(bool bEnable)
{
    bEnable == true ? OutlineComponent->EnableOutline() : OutlineComponent->DisableOutline();
}


void AGN_Character::SetEnemyState(EEnemyState NewState)
{
    if (CurrentState == EEnemyState::Dead)
        return;

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

    //SetEnemyState(EEnemyState::Attacking);
    //PlayAttackAnimation();


    PreAnimation = AttackAnimation;
    CurrentState = EEnemyState::Attacking;
    AIController->RemovePatrolling();

    

    GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(ScreamTimerHandle);
    //AttackAnimation

    //GetWorld()->GetTimerManager().SetTimer(
    //    PatrolTimerHandle,
    //    this,
    //    &AGN_Character::StartChase,  // 1.5초 후 다시 추격 시작
    //    1.5f,
    //    false
    //);
}

void AGN_Character::Die()
{
    //SetEnemyState(EEnemyState::Dead);
    //PlayDeathAnimation();
    
    PreAnimation = DeathAnimation;
    CurrentState = EEnemyState::Dead;

    GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(ScreamTimerHandle);
    
    PawnSensingComp->OnSeePawn.RemoveDynamic(this, &AGN_Character::OnSeePawn);
    AIController->RemovePatrolling();
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

void AGN_Character::Dead()
{
    GetWorldTimerManager().ClearTimer(DeadAnimTimerHandle);
    Destroy();
}

void AGN_Character::AttackEnd()
{
    
    CurrentState = EEnemyState::Chasing;
}

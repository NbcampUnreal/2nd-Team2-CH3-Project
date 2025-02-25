#include "GN_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UGN_AnimInstance::UpdateAnimationProperties()
{
    APawn* OwnerPawn = TryGetPawnOwner();

    if (OwnerPawn)
    {
        Speed = OwnerPawn->GetVelocity().Size();

        bIsRunning = Speed > 400.0f;
    }
}
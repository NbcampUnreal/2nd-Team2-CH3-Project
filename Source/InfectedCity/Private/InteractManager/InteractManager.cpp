#include "InteractManager/InteractManager.h"
#include "Interactable/Interactable.h"

void UInteractManager::AddInteractActor(FName Key, AActor* NewActor)
{
    if (NewActor && NewActor->Implements<UInteractable>())
    {
        InteractableActors.Add(Key, NewActor);
    }
}

void UInteractManager::RemoveInteractActor(FName Key)
{
    InteractableActors.Remove(Key);
}

void UInteractManager::InteractWith(FName Key)
{
    TArray<AActor*> Actors;
    InteractableActors.MultiFind(Key, Actors);

    for (AActor* Actor : Actors)
    {
        if (IInteractable* Interactable = Cast<IInteractable>(Actor))
        {
            Interactable->OnInteract();
        }
    }
}
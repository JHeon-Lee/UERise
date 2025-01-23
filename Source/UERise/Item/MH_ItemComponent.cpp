// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetGuidLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

const FName UMH_ItemComponent::TAG_ITEM = FName("Item");

UMH_ItemComponent::UMH_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetAutoActivate(true);

    ItemName = TAG_ITEM;
}

void UMH_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemId);
	DOREPLIFETIME(ThisClass, InventoryId);
	DOREPLIFETIME(ThisClass, OwnerId);
	DOREPLIFETIME(ThisClass, MaxStack);
	DOREPLIFETIME(ThisClass, CurrentStack);
	DOREPLIFETIME(ThisClass, ItemName);
	DOREPLIFETIME(ThisClass, ItemTagSlotType);
}


// Called when the game starts
void UMH_ItemComponent::BeginPlay()
{
	Super::BeginPlay();	

    if (UKismetSystemLibrary::IsServer(this))
    {
        if (!ItemId.IsValid())
        {
            ItemId = UKismetGuidLibrary::NewGuid();
        }
    }

    AActor* ItemComponentOwner = GetOwner();
    if (!ensure(IsValid(ItemComponentOwner)))
    {
        return;
    }

    ItemComponentOwner->bNetUseOwnerRelevancy = true;

    ItemComponentOwner->Tags.AddUnique(TAG_ITEM);
}


// Called every frame
void UMH_ItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_ItemComponent::PickUpItem(UMH_ItemManager* InventoryPick)
{
    AActor* ItemActor = GetOwner();
    if (!IsValid(ItemActor) || !ItemActor->HasAuthority())
    {
        return;
    }

    if (!IsValid(InventoryPick))
    {
        return;
    }

    /* Only do for items not already in same inventory */
    if (InventoryId == InventoryPick->InventoryId)
    {
        return;
    }

    // Check for valid inventory owner as actor, to set ownership of items
    AActor* InventoryOwner = InventoryPick->GetOwner();
    if (!IsValid(InventoryOwner))
    {
        return;
    }

    if (InventoryPick->AddItemsOfClass(ItemActor->GetClass(), CurrentStack))
    {
        // Set ownership and instigator for the item - so the dispatcher can notify the owner of the update.
        ItemActor->SetOwner(InventoryOwner);
        ItemActor->SetInstigator(InventoryOwner->GetInstigator());
        OnPickup.Broadcast(InventoryPick);
        ItemActor->Destroy();
    }
}

void UMH_ItemComponent::UseItem(AActor* User, FGameplayTag GameplayTag)
{
    const AActor* ItemComponentOwner = GetOwner();
    if (!IsValid(ItemComponentOwner) || !ItemComponentOwner->HasAuthority())
    {
        return;
    }

    OnItemUsed.Broadcast(User, GameplayTag);
}



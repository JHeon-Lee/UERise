// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MH_ItemManager.h"
#include "Item/MH_ItemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetGuidLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UMH_ItemManager::UMH_ItemManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetIsReplicatedByDefault(true);
	UMH_ItemManager::SetAutoActivate(true);
}


// Called when the game starts
void UMH_ItemManager::BeginPlay()
{
	Super::BeginPlay();

	InventoryStorage = nullptr;

	if (!InventoryId.IsValid())
	{
		InventoryId = FGuid::NewGuid();
	}

    SetupInventoryStorageReference();
}

// Called every frame
void UMH_ItemManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_ItemManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryId);
	DOREPLIFETIME(ThisClass, InventoryStorage);
}

bool UMH_ItemManager::AddItemsOfClass(const TSubclassOf<AActor> Class, const int32 Quantity)
{
	AActor* InventoryManagerOwner = GetOwner();
	if (!IsValid(InventoryManagerOwner) || !InventoryManagerOwner->HasAuthority())
	{
		return false;
	}

	if (Quantity <= 0)
	{
		return false;
	}

	TArray<AActor*> ItemArray;
	if (GetAllItemsOfClass(Class, ItemArray))
	{
		for (AActor* ItemActor : ItemArray)
		{
			UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ? 
				Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;
			
			if (IsValid(ItemComponent))
			{
                ItemComponent->CurrentStack += Quantity;

                OnItemUpdated.Broadcast(ItemActor);
                return true;
			}
		}
	}

	return true;
}

bool UMH_ItemManager::RemoveItemsOfClass(const TSubclassOf<AActor> Class, const int32 Quantity)
{
    const AActor* InventoryManagerOwner = GetOwner();
    if (!IsValid(InventoryManagerOwner) || !InventoryManagerOwner->HasAuthority())
    {
        return false;
    }

    const bool bHasEnoughItems = HasEnoughItems(Class, Quantity);

    if (!bHasEnoughItems)
    {
        return false;
    }

    TArray<AActor*> ItemArray;
    if (!GetAllItemsOfClass(Class, ItemArray))
    {
        return false;
    }

    for (int32 i = ItemArray.Num() - 1; i >= 0; --i)
    {
        AActor* ItemActor = ItemArray[i];
        if (!IsValid(ItemActor))
        {
            continue;
        }

        UMH_ItemComponent* ItemComponent = Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass()));

        if (IsValid(ItemComponent))
        {
            if (ItemComponent->CurrentStack > Quantity)
            {
                ItemComponent->CurrentStack -= Quantity;
            }
            else
            {
                ItemActor->Destroy();
                return true;
            }
        }
    }
    return true;
}

TArray<AActor*> UMH_ItemManager::GetAllItems()
{
    TArray<AActor*> Items;

    SetupInventoryStorageReference();

    if (!IsValid(InventoryStorage))
    {
        return Items;
    }

    InventoryStorage->GetAttachedActors(Items, true);

    return Items;
}

bool UMH_ItemManager::GetAllItemsOfClass(const TSubclassOf<AActor> Class, TArray<AActor*>& OutFilteredArray)
{
	TArray<AActor*> FilteredArray;
	UKismetArrayLibrary::FilterArray(GetAllItems(), Class, FilteredArray);
	if (FilteredArray.Num() > 0)
	{
		OutFilteredArray = FilteredArray;
		return true;
	}

	return false;
}

bool UMH_ItemManager::HasEnoughItems(const TSubclassOf<AActor> Item, const int32 Quantity)
{
    if (Quantity <= 0)
    {
        return false;
    }

    TArray<AActor*> ItemArray;
    if (!GetAllItemsOfClass(Item, ItemArray))
    {
        return false;
    }

    for (int32 i = 0; i < ItemArray.Num(); ++i)
    {
        const AActor* ItemActor = ItemArray[i];

        UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ?
            Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

        if (!ensure(IsValid(ItemComponent)))
        {
            continue;
        }

        if (Quantity > ItemComponent->CurrentStack)
        {
            return true;
        }
    }

	return false;
}

bool UMH_ItemManager::HasExactItem(AActor* Item)
{
    return GetAllItems().Contains(Item);
}

void UMH_ItemManager::SetupInventoryStorageReference()
{
    if (IsValid(InventoryStorage))
    {
        return;
    }

    AActor* InventoryOwner = GetOwner();
    if (!IsValid(InventoryOwner))
    {
        return;
    }

    InventoryStorage = InventoryOwner;    
}

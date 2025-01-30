// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MH_ItemManager.h"
#include "Item/MH_ItemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/SphereComponent.h"
#include "Interface/MHItemInferface.h"
#include "Kismet/KismetMathLibrary.h"
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

	if (!InventoryId.IsValid())
	{
		InventoryId = FGuid::NewGuid();
	}

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
    
    AActor* ItemActor = nullptr;

    // Already has Item
	if (GetItemOfClass(Class, ItemActor))
	{
	    UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ? 
	    	Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;
	    
	    if (IsValid(ItemComponent))
	    {
           ItemComponent->CurrentStack += Quantity;

           UE_LOG(LogTemp, Log, TEXT("Add Item Of Class : %s, Current Stack : %d "), *ItemActor->GetName(), ItemComponent->CurrentStack);

           OnItemUpdated.Broadcast(ItemActor);
           return true;
	    }
	}
    // Add New
    else
    {
        const AActor* ItemCDO = Cast<AActor>(Class->StaticClass()->GetDefaultObject());
        FTransform NewTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, -500), FVector(1, 1, 1));
        //NewTransform.SetScale3D(IsValid(ItemCDO) ? ItemCDO->GetActorScale() : FVector::OneVector);
        AActor* NewItemActor = GetWorld()->SpawnActor(Class, &NewTransform);      


        if (IsValid(NewItemActor))
        {
            UE_LOG(LogTemp, Log, TEXT("Add new Items Of Class : %s"), *NewItemActor->GetName());

            NewItemActor->SetOwner(GetOwner());
            NewItemActor->SetInstigator(GetOwner()->GetInstigator());

            UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(NewItemActor->GetRootComponent());
            if (IsValid(PrimitiveComponent))
            {
                PrimitiveComponent->SetSimulatePhysics(false);
            }
            NewItemActor->SetActorHiddenInGame(true);
            NewItemActor->SetActorEnableCollision(false);

            InventoryStorage.Add(NewItemActor);
            OnItemUpdated.Broadcast(NewItemActor);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Add Items Of Class Failed, ItemActor Is Not Valid"));
            return false;
        }
    }

	return false;
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

    AActor* ItemActor;
    if (GetItemOfClass(Class, ItemActor))
    {
        UMH_ItemComponent* ItemComponent = Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass()));

        if (IsValid(ItemComponent))
        {
            if (ItemComponent->CurrentStack >= Quantity)
            {
                ItemComponent->CurrentStack -= Quantity;
            }
            else
            {
            //    ItemActor->Destroy();
            }

            // Notify To Widget
            OnItemRemoved.Broadcast(ItemActor);
            return true;
        }
    }

    return false;
}

bool UMH_ItemManager::RemoveItemsOfTag(const FGameplayTag Tag, const int32 Quantity)
{
    const AActor* InventoryManagerOwner = GetOwner();
    if (!IsValid(InventoryManagerOwner) || !InventoryManagerOwner->HasAuthority())
    {
        return false;
    }

    const bool bHasEnoughItems = HasEnoughItemsWithTag(Tag, Quantity);

    if (!bHasEnoughItems)
    {
        return false;
    }

    AActor* ItemActor;
    if (GetItemOfTag(Tag, ItemActor))
    {
        UMH_ItemComponent* ItemComponent = Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass()));

        if (IsValid(ItemComponent))
        {
            if (ItemComponent->CurrentStack >= Quantity)
            {
                ItemComponent->CurrentStack -= Quantity;
            }
            else
            {
            //    ItemActor->Destroy();
            }

            // Notify To Widget
            OnItemRemoved.Broadcast(ItemActor);
            return true;
        }
    }
    return false;
}

TArray<AActor*> UMH_ItemManager::GetAllItems()
{
    return InventoryStorage;
}

bool UMH_ItemManager::GetItemOfClass(const TSubclassOf<AActor> Class, AActor*& ItemActor)
{
    if (InventoryStorage.IsEmpty())
    {
        return false;
    }

    for (const auto& Actor : InventoryStorage)
    {
        if (IsValid(Actor) && Actor->IsA(Class))
        {
            ItemActor = Actor;
            return true;
        }
    }

	return false;
}

bool UMH_ItemManager::GetItemOfTag(FGameplayTag ItemTag, AActor*& ItemActor)
{
    if (InventoryStorage.IsEmpty())
    {
        return false;
    }

    for (const auto& Item : InventoryStorage)
    {
        UMH_ItemComponent* ItemComponent = Item->FindComponentByClass<UMH_ItemComponent>();

        if (ItemComponent && ItemComponent->ItemTagSlotType == ItemTag)
        {
            ItemActor = Item;
            return true;
        }
    }

    return false;
}

bool UMH_ItemManager::HasEnoughItems(const TSubclassOf<AActor> Item, const int32 Quantity)
{
    if (Quantity <= 0)
    {

        return false;
    }

    AActor* ItemActor;
    if (!GetItemOfClass(Item, ItemActor))
    {
        return false;
    }

    UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ?
        Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

    if (!ensure(IsValid(ItemComponent)))
    {
        return false;
    }

    if (Quantity > ItemComponent->CurrentStack)
    {
        return true;
    }

	return false;
}

bool UMH_ItemManager::HasEnoughItemsWithTag(FGameplayTag ItemTag, const int32 Quantity)
{
    if (Quantity <= 0)
    {
        return false;
    }

    AActor* ItemActor;
    if (!GetItemOfTag(ItemTag, ItemActor))
    {
        return false;
    }

    UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ?
        Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

    if (!ensure(IsValid(ItemComponent)))
    {
        return false;
    }

    if (ItemComponent->CurrentStack >= Quantity)
    {
        return true;
    }


    return false;
}

int32 UMH_ItemManager::GetItemQuantity(const TSubclassOf<AActor> Item)
{
    AActor* ItemActor;
    if (!GetItemOfClass(Item, ItemActor))
    {
        return -1;
    }

    UMH_ItemComponent* ItemComponent = IsValid(ItemActor) ?
        Cast<UMH_ItemComponent>(ItemActor->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

    if (ItemComponent->CurrentStack > 0)
    {
        return ItemComponent->CurrentStack;
    }

    return -1;
}

bool UMH_ItemManager::HasExactItem(AActor* Item)
{
    return GetAllItems().Contains(Item);
}

bool UMH_ItemManager::UseSelectedConsumble(FGameplayTag ItemTag)
{
    AActor* item = nullptr;
    GetItemOfTag(ItemTag, item);

    if (item)
    {
        UMH_ItemComponent* ItemComponent = IsValid(item) ?
            Cast<UMH_ItemComponent>(item->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

        if (ItemComponent->ItemTagSlotType == ItemTag)
        {
            if (RemoveItemsOfTag(ItemTag, 1))
            {
                UE_LOG(LogTemp, Log, TEXT("Item Removed"));
            }
            ItemComponent->UseItem(item, ItemTag);            
            
        }
    }

    return false;
}

bool UMH_ItemManager::StopItemUse(const FGameplayTag ItemTag)
{
    AActor* item = nullptr;
    bool bItemHasTag = GetItemOfTag(ItemTag, item);

    if (!bItemHasTag)
    {
        UE_LOG(LogTemp, Log, TEXT("No Tag"));
    }

    if (item && bItemHasTag)
    {
        UMH_ItemComponent* ItemComponent = IsValid(item) ?
            Cast<UMH_ItemComponent>(item->GetComponentByClass(UMH_ItemComponent::StaticClass())) : nullptr;

        if (ItemComponent)
        {
            if (ItemComponent->ItemTagSlotType == ItemTag)
            {
                ItemComponent->StopUseItem(item);
                return true;
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("No ItemComponent"));
        }
    }

    return false;
}

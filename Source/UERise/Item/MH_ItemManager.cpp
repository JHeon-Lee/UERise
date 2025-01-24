// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MH_ItemManager.h"
#include "Item/MH_ItemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/SphereComponent.h"
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

           OnItemUpdated.Broadcast(ItemActor);
           return true;
	    }
	}
    // Add New
    else
    {
        const AActor* ItemCDO = Cast<AActor>(Class->StaticClass()->GetDefaultObject());
        FTransform NewTransform = GetOwner()->GetActorTransform();
        NewTransform.SetScale3D(IsValid(ItemCDO) ? ItemCDO->GetActorScale() : FVector::OneVector);
        AActor* NewItemActor = GetWorld()->SpawnActor(Class, &NewTransform);      


        if (IsValid(NewItemActor))
        {
            UE_LOG(LogTemp, Log, TEXT("Add new Valid Items Of Class"));

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
            UE_LOG(LogTemp, Log, TEXT("Add new Valid Items Of Class Failed"));
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

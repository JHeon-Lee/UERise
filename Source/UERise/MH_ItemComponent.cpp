// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetGuidLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UMH_ItemComponent::UMH_ItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetAutoActivate(true);
	// ...
}

void UMH_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemId);
	DOREPLIFETIME(ThisClass, InventoryId);
	DOREPLIFETIME(ThisClass, OwnerId);
	DOREPLIFETIME(ThisClass, bStackable);
	DOREPLIFETIME(ThisClass, MaxStack);
	DOREPLIFETIME(ThisClass, CurrentStack);
	DOREPLIFETIME(ThisClass, ItemName);
	DOREPLIFETIME(ThisClass, bSimulateWhenDropped);
	DOREPLIFETIME(ThisClass, ItemTagSlotType);
}


// Called when the game starts
void UMH_ItemComponent::BeginPlay()
{
	Super::BeginPlay();	

	if (AActor* ItemComponentOwner = GetOwner())
	{
		ItemComponentOwner->Tags.AddUnique(ItemName);
	}	
}


// Called every frame
void UMH_ItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_ItemComponent::PickUpItem(AActor* InventoryPicking)
{

}

void UMH_ItemComponent::DropItem()
{
}

void UMH_ItemComponent::UseItem(AActor* User, FGameplayTag GameplayTag)
{
}

bool UMH_ItemComponent::SplitItems(const int32 SplitSize)
{
	return false;
}

void UMH_ItemComponent::DestroyItem()
{
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MHItemFrameWidget.h"
#include "MHGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/MH_ItemComponent.h"
#include "Interface/MHWidgetInterface.h"


UMHItemFrameWidget::UMHItemFrameWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMHItemFrameWidget::ItemUpdate(AActor* UpdatedItem)
{
	UE_LOG(LogTemp, Log, TEXT("ItemUpdate Called"));
	UMH_ItemComponent* ItemComponent = UpdatedItem->FindComponentByClass<UMH_ItemComponent>();

	if (IsValid(ItemComponent))
	{
		bool bIsItemConsumable = ItemComponent->ItemTagSlotType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable")));

		// Item Is Not Consumable
		if(!bIsItemConsumable)
		{
			return;
		}

		// ConsuableArray is Empty, So Add New Actor Directly
		if (ConsumableArray.IsEmpty())
		{
			ConsumableArray.Add(UpdatedItem);
			DisplayingIndex = ConsumableArray.Num() - 1;
			UpdateIconAndQunatity();

			return;
		}

		// If Item Actor already in Consumable Array, add Stack
		for (int32 index = 0; index < ConsumableArray.Num(); index++)
		{
			if (ConsumableArray[index]->GetClass() == UpdatedItem->GetClass())
			{
				if (index == DisplayingIndex)
				{
					UpdateIconAndQunatity();
				}
				return;
			}
		}

		// Add New Item to Item Slot
		ConsumableArray.Add(UpdatedItem);
		DisplayingIndex = ConsumableArray.Num()-1;
	}
}

void UMHItemFrameWidget::ConsumeItemUpdate(AActor* UpdatedItem)
{
	UE_LOG(LogTemp, Log, TEXT("ConsumeItemUpdate Called"));
	UMH_ItemComponent* ItemComponent = UpdatedItem->FindComponentByClass<UMH_ItemComponent>();

	if (IsValid(ItemComponent))
	{		
		// Find Item Actor in Consumable Array, Remove Stack
		for (int32 index = 0; index < ConsumableArray.Num(); index++)
		{
			if (ConsumableArray[index]->GetClass() == UpdatedItem->GetClass())
			{
				if (index == DisplayingIndex)
				{
					UpdateIconAndQunatity();
				}
				return;
			}
		}
	}
}

const FGameplayTag UMHItemFrameWidget::GetDisplayingItemTag()
{
	FGameplayTag Tag;
	if (ConsumableArray.IsValidIndex(DisplayingIndex))
	{
		UMH_ItemComponent* ItemComponent = ConsumableArray[DisplayingIndex]->FindComponentByClass<UMH_ItemComponent>();
		if (ItemComponent)
		{
			return ItemComponent->ItemTagSlotType;
		}
	}

	return FGameplayTag();
}

void UMHItemFrameWidget::ChangeDisplayingIndex(float InputValue)
{
	if (ConsumableArray.Num() <= 1)
	{
		return;
	}

	if (InputValue > 0)
	{
		DisplayingIndex++;

		if (DisplayingIndex >= ConsumableArray.Num())
		{
			DisplayingIndex -= ConsumableArray.Num();
		}		
	}
	else if (InputValue < 0)
	{
		DisplayingIndex--;

		if (DisplayingIndex <= -1)
		{
			DisplayingIndex += ConsumableArray.Num();
		}
	}

	UpdateIconAndQunatity();
}

void UMHItemFrameWidget::UpdateIconAndQunatity()
{
	if (ConsumableArray.IsValidIndex(DisplayingIndex))
	{
		UMH_ItemComponent* ItemComponent = ConsumableArray[DisplayingIndex]->FindComponentByClass<UMH_ItemComponent>();
		UMHGameInstance* GameInstance = Cast<UMHGameInstance>(GetWorld()->GetGameInstance());

		if (ItemComponent && GameInstance)
		{
			ConsumableIcon->SetBrush(GameInstance->FindImage(ItemComponent->ItemTagSlotType.GetTagName()));
			Quantity->SetText(FText::AsNumber(ItemComponent->CurrentStack));
		}				
	}

}

void UMHItemFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConsumableIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));

	Quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("Number")));

	
	if (GetOwningPlayerPawn())
	{
		UE_LOG(LogTemp, Log, TEXT("Owning Actor Is null"));
	}


	IMHWidgetInterface* CharacterWidget = Cast<IMHWidgetInterface>(OwingActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupItemWidget(this);
	}
}
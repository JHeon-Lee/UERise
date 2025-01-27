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
		bool bIsItemConsumable = ItemComponent->ItemTagSlotType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Consumable")));

		// Item Is Not Consumable
		if(!bIsItemConsumable)
		{
			return;
		}

		// ConsuableArray is Empty, So Add Actor Directly
		if (ConsumableArray.IsEmpty())
		{
			ConsumableArray.Add(UpdatedItem);

			UMHGameInstance* GameInstance = Cast<UMHGameInstance>(GetWorld()->GetGameInstance());
			if (IsValid(GameInstance))
			{
				ConsumableIcon->SetBrush(GameInstance->FindImage(ItemComponent->ItemTagSlotType.GetTagName()));
			}			
			Quantity->SetText(FText::AsNumber(ItemComponent->CurrentStack));

			return;
		}

		// If Item Actor already in Consumable Array, add Stack
		for (AActor* item : ConsumableArray)
		{
			if (item->GetClass() == UpdatedItem->GetClass())
			{

				return;
			}
		}

		ConsumableArray.Add(UpdatedItem);
	}


}

void UMHItemFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConsumableIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));

	Quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("Number")));

	if (!IsValid(OwingActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Owning Actor Is null"));
	}
		

	IMHWidgetInterface* CharacterWidget = Cast<IMHWidgetInterface>(OwingActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupItemWidget(this);
	}

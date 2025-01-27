// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MHHUDWidget.h"
#include "UI/MHItemFrameWidget.h"
#include "UI/UtusiHPBarWidget.h"
#include "GameplayTagContainer.h"
#include "Interface/MHWidgetInterface.h"

UMHHUDWidget::UMHHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMHHUDWidget::UpdateMaxHp(float MaxHp)
{
	HpBar->SetMaxHp(MaxHp);
}

void UMHHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UMHHUDWidget::UpdateItemSlotBar(AActor* NewItem)
{
	ItemWidget->ItemUpdate(NewItem);
}

void UMHHUDWidget::ChangeItemSlot(float InputValue)
{
	ItemWidget->ChangeDisplayingIndex(InputValue);
}



void UMHHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UUtusiHPBarWidget>(GetWidgetFromName(TEXT("WidgetHPBar")));
	ensure(HpBar);

	ItemWidget = Cast<UMHItemFrameWidget>(GetWidgetFromName(TEXT("ItemFrame")));
	ensure(ItemWidget);

	IMHWidgetInterface* HUDPawn = Cast<IMHWidgetInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUD(this);
	}




}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UtusiHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/MHWidgetInterface.h"

UUtusiHPBarWidget::UUtusiHPBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxHP = -1.0f;

}

void UUtusiHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	if (HpProgressBar)
	{
		UE_LOG(LogTemp, Log, TEXT("HPBar Cast Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("HPBar Cast Failed"));
	}

	IMHWidgetInterface* CharacterWidget = Cast<IMHWidgetInterface>(OwingActor);
	if (CharacterWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("UUtusiHPBarWidget OwingActor Cast Success"));
		CharacterWidget->SetupCharacterHPWidget(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UUtusiHPBarWidget OwingActor Cast Failed"));
	}
}

void UUtusiHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	
	ensure(MaxHP > 0.0f);
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHP);
		UE_LOG(LogTemp, Log, TEXT("Hp %f Update"), (NewCurrentHp / MaxHP));
	}


}

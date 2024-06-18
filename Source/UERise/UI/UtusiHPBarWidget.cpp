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

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UtusiHpBar")));
	ensure(HpProgressBar);
	
	IMHWidgetInterface* CharacterWidget = Cast<IMHWidgetInterface>(OwingActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("sdsdsdsdsd"));
	}
}

void UUtusiHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHP > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHP);


	}
}

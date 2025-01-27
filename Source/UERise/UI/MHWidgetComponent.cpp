// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MHWidgetComponent.h"
#include "UI/MHUserWidget.h"
#include "UI/UtusiHPBarWidget.h"

void UMHWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UE_LOG(LogTemp, Log, TEXT("UMHWidgetComponent InitWidget Called"));

	UMHUserWidget* MHUserWidget = Cast<UMHUserWidget>(GetWidget());
	if (MHUserWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("MHUserWidget Cast Success"));
		MHUserWidget->SetOwningActor(GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MHUserWidget Cast Failed"));
	}
}

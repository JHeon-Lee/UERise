// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MHWidgetComponent.h"
#include "UI/MHUserWidget.h"

void UMHWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UMHUserWidget* MHUserWidget = Cast<UMHUserWidget>(GetWidget());
	if (MHUserWidget)
	{
		MHUserWidget->SetOwningActor(GetOwner());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MHPlayerController.h"
#include "UI/MHHUDWidget.h"

AMHPlayerController::AMHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMHHUDWidget> MHHUDWidgetRef(TEXT("/Game/Characters/Utusi/UI/WBP_MHHUD.WBP_MHHUD_C"));
	if (MHHUDWidgetRef.Class)
	{
		MHHUDWidgetClass = MHHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UMHHUDWidget> MHVillageHUDWidgetRef(TEXT("/Game/Characters/Utusi/UI/WBP_MHVillageHUD.WBP_MHVillageHUD_C"));
	if (MHVillageHUDWidgetRef.Class)
	{
		MHVillageHUDWidgetClass = MHVillageHUDWidgetRef.Class;
	}
}

void AMHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);


	MHHUDWidget = CreateWidget<UMHHUDWidget>(this, MHHUDWidgetClass);
	if(MHHUDWidget)
	{
		MHHUDWidget->AddToViewport();
	}

}

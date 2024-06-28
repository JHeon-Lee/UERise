// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMHPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AMHPlayerController();

//HUD
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UMHHUDWidget> MHHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UMHHUDWidget> MHHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UMHHUDWidget> MHVillageHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UMHHUDWidget> MHVillageHUDWidget;
};

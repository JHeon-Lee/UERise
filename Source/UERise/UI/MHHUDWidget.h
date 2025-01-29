// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/MHItemFrameWidget.h"
#include "GameplayTagContainer.h"
#include "MHHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHHUDWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UMHHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
	void UpdateMaxHp(float MaxHp);

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	UFUNCTION()
	void UpdateItemSlotBar(AActor* NewItem);

	void ChangeItemSlot(float InputValue);

	FGameplayTag GetDisplayingItemTag() {return ItemWidget->GetDisplayingItemTag(); };

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UUtusiHPBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UMHItemFrameWidget> ItemWidget;


};

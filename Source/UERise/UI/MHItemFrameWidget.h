// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MHUserWidget.h"
#include "GameplayTagContainer.h"
#include "MHItemFrameWidget.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHItemFrameWidget : public UMHUserWidget
{
	GENERATED_BODY()

public:
	UMHItemFrameWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void ItemUpdate(AActor* UpdatedItem);

	UFUNCTION()
	void ConsumeItemUpdate(AActor* UpdatedItem);

	const FGameplayTag GetDisplayingItemTag();

	void ChangeDisplayingIndex(float InputValue);
	void UpdateIconAndQunatity();

protected:
	virtual void NativeConstruct() override;

public:
	int32 DisplayingIndex = -1;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> ConsumableArray;

	UPROPERTY()
	TObjectPtr<class UImage> ConsumableIcon;

	UPROPERTY()
	TObjectPtr<class UTextBlock> Quantity;
	
};

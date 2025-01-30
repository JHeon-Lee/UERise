// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MH_ItemBase.h"
#include "MH_ItemHoney.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMH_ItemHoney : public AMH_ItemBase
{
	GENERATED_BODY()
	
protected:
		
	virtual void UseItem(AActor* Item, FGameplayTag ItemTag) override;
	virtual void StopUse(AActor* Item) override;
		
	void ApplyEffectTick();
	void EndEffect();

private:
	FTimerHandle EffectTimerHandle;
	FTimerHandle EndEffectTimerHandle;

	TObjectPtr<class AActor> ItemOwner;
};

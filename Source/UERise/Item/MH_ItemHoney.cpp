// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MH_ItemHoney.h"
#include "Character/Component/MH_PlayerStatComponent.h"
#include "MH_ItemHoney.h"

void AMH_ItemHoney::UseItem(AActor* Item, FGameplayTag ItemTag)
{
	Super::UseItem(Item, ItemTag);

	UE_LOG(LogTemp, Log, TEXT("Honey Item Called"));

	ItemOwner = Item->GetOwner();
	ensure(ItemOwner);

	UMH_PlayerStatComponent* OwnerStat = ItemOwner->FindComponentByClass<UMH_PlayerStatComponent>();
	ensure(OwnerStat);

	if (OwnerStat)
	{
		GetWorldTimerManager().SetTimer(EffectTimerHandle,this,&AMH_ItemHoney::ApplyEffectTick,0.2f,true);

		GetWorldTimerManager().SetTimer(EndEffectTimerHandle,this,&AMH_ItemHoney::EndEffect,3.0f,false);
	}

}

void AMH_ItemHoney::StopUse(AActor* Item)
{
	EndEffect();
}

void AMH_ItemHoney::ApplyEffectTick()
{
	UMH_PlayerStatComponent* OwnerStat = ItemOwner->FindComponentByClass<UMH_PlayerStatComponent>();
	OwnerStat->SetCurrentHp(OwnerStat->GetCurrentHp() + 1.6f);
}

void AMH_ItemHoney::EndEffect()
{
	GetWorldTimerManager().ClearTimer(EffectTimerHandle);
}

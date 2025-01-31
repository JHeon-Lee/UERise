// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/MHDataContainer.h"
#include "MHGameInstance.generated.h"

#define MHLOG(Format) UE_LOG(LogTemp, Log, TEXT(Format))
#define FIND_MONTAGE(Tag) Cast<UMHGameInstance>(GetOwner()->GetWorld()->GetGameInstance())->FindMontage(Tag)
#define FIND_SYSTEM(Tag)  Cast<UMHGameInstance>(GetOwner()->GetWorld()->GetGameInstance())->FindSystem(Tag)
/**
 * 
 */

UCLASS()
class UERISE_API UMHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAnimMontage* FindMontage(FName Tag)
	{
		FGameplayTag GameTag = FGameplayTag::RequestGameplayTag(Tag);
		if (GameTag.IsValid())
		{			
			return MHDataContainerAsset->GetAnimationMontage(GameTag);
		}
		return nullptr;
	}

	UNiagaraSystem* FindSystem(FName Tag)
	{
		return MHDataContainerAsset->GetNiagaraSystem(FGameplayTag::RequestGameplayTag(Tag));
	}

	FSlateBrush FindImage(FName Tag)
	{
		if (FGameplayTag::RequestGameplayTag(Tag).IsValid())
		{
			return MHDataContainerAsset->GetBrush(FGameplayTag::RequestGameplayTag(Tag));
		}

		return FSlateBrush();
	}

	UCurveFloat* FindCurveFloat(FName Tag)
	{
		FGameplayTag GameTag = FGameplayTag::RequestGameplayTag(Tag);
		if (GameTag.IsValid())
		{
			return MHDataContainerAsset->GetCurveFloat(GameTag);
		}
		return nullptr;
	}

	USoundBase* FindSoundBase(FName Tag)
	{
		FGameplayTag GameTag = FGameplayTag::RequestGameplayTag(Tag);
		if (GameTag.IsValid())
		{
			return MHDataContainerAsset->GetSoundBase(GameTag);
		}
		return nullptr;
	}

protected:
	UMHGameInstance();
	UMHDataContainer* GetMHDataContainerAsset() { return MHDataContainerAsset; }		
		

protected:
	TObjectPtr<UMHDataContainer> MHDataContainerAsset;
	
};

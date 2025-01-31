// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "SlateCore/Public/Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundBase.h"
#include "Sound/DialogueWave.h"
#include "Curves/CurveFloat.h"
#include "MHDataContainer.generated.h"
/**
 * 
 */
UCLASS()
class UERISE_API UMHDataContainer : public UDataAsset
{
	GENERATED_BODY()

    
public:
    UMHDataContainer();

    UAnimMontage* GetAnimationMontage(FGameplayTag Key) const
    {
        if (AnimationMontageMap.Contains(Key))
        {
            return AnimationMontageMap[Key];
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("MontageNotFound , TagName : %s"), *Key.GetTagName().ToString());
        }
        return nullptr; 
    }

    UNiagaraSystem* GetNiagaraSystem(FGameplayTag Key) const
    {
        if (NiagaraEffectMap.Contains(Key))
        {
            return NiagaraEffectMap[Key];
        }
        return nullptr;
    }

    FSlateBrush GetBrush(FGameplayTag Key) const
    {
        if (BrushMap.Contains(Key))
        {
            return BrushMap[Key];
        }
        return FSlateBrush();
    }

    UCurveFloat* GetCurveFloat(FGameplayTag Key) const
    {
        if (CurveFloatMap.Contains(Key))
        {
            return CurveFloatMap[Key];
        }
        return nullptr;
    }

    USoundBase* GetSoundBase(FGameplayTag Key) const
    {
        if (SoundBaseMap.Contains(Key))
        {
            return SoundBaseMap[Key];
        }
        return nullptr;
    }


public:
    // Montage Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TMap<FGameplayTag, UAnimMontage*> AnimationMontageMap;

    // Niagara Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TMap<FGameplayTag, class UNiagaraSystem*> NiagaraEffectMap;

    // Image Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Images")
    TMap<FGameplayTag, FSlateBrush> BrushMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves")
    TMap<FGameplayTag, class UCurveFloat*> CurveFloatMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves")
    TMap<FGameplayTag, class USoundBase*> SoundBaseMap;

    
    UFUNCTION(BlueprintCallable, Category = "Game Data")
    void InitializeData();
	
};

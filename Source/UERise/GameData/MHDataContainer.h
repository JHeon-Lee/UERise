// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
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
            UE_LOG(LogTemp, Log, TEXT("MontageNotFound"));
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



public:
    // 애니메이션 몽타주 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TMap<FGameplayTag, UAnimMontage*> AnimationMontageMap;

    // 나이아가라 이펙트 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TMap<FGameplayTag, class UNiagaraSystem*> NiagaraEffectMap;

    // 데이터 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Game Data")
    void InitializeData();
	
};

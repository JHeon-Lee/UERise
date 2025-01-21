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
    // �ִϸ��̼� ��Ÿ�� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TMap<FGameplayTag, UAnimMontage*> AnimationMontageMap;

    // ���̾ư��� ����Ʈ ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TMap<FGameplayTag, class UNiagaraSystem*> NiagaraEffectMap;

    // ������ �ʱ�ȭ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Game Data")
    void InitializeData();
	
};

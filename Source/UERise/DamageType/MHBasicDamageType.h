// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "GameplayTagContainer.h"
#include "MHBasicDamageType.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHBasicDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageTag; 
};

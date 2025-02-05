// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MHBasicDamageType.h"
#include "MHDamageType_Flash.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHDamageType_Flash : public UMHBasicDamageType
{
	GENERATED_BODY()
	
public:
	UMHDamageType_Flash() { DamageTag = FGameplayTag::RequestGameplayTag(FName("DamageType.Flash")); };
};

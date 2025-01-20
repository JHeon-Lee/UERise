// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MHBasicDamageType.h"
#include "MHDamageType_StunTrap.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHDamageType_StunTrap : public UMHBasicDamageType
{
	GENERATED_BODY()
	
public:
	UMHDamageType_StunTrap() { DamageTag = FGameplayTag::RequestGameplayTag(FName("DamageType.Trap")); };
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MHBasicDamageType.h"
#include "MHDamageType_Capture.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHDamageType_Capture : public UMHBasicDamageType
{
	GENERATED_BODY()
	
public:
	UMHDamageType_Capture() { DamageTag = FGameplayTag::RequestGameplayTag(FName("DamageType.Capture")); };
};

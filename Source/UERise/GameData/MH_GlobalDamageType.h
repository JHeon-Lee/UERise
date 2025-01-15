// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MH_GlobalDamageType.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMH_GlobalDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsRoarAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsCaptureAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsFlashAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsTackleAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsTrap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	bool bIsSleepAttack;
};

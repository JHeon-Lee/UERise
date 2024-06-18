// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MHMonsterStat.h"
#include "MHGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMHGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class UERISE_API UMHGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMHGameSingleton();
	static UMHGameSingleton& Get();

public:
	FORCEINLINE FMHMonsterStat GetMonsterStat(float InLevel) const { return MonsterStatTable.IsValidIndex(InLevel - 1) ? MonsterStatTable[InLevel - 1] : FMHMonsterStat(); }

	UPROPERTY()
	float MonstersNum;

private:
	TArray<FMHMonsterStat> MonsterStatTable;
};

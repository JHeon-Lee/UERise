
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MHMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FMHMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMHMonsterStat() : MaxHp(0.0f), Pattern1(0.0f), Pattern2(0.0f), Pattern3(0.0f),
		Pattern4(0.0f), Pattern5_1(0.0f), Pattern5_2(0.0f), Pattern5_3(0.0f),
		Pattern6(0.0f), Pattern7(0.0f), Pattern8(0.0f), Pattern9(0.0f),
		HeadBreakThreshold(0.0f), LegBreakThreshold(0.0f), TailCutThreshold(0.0f),
		WingBreakThreshold(0.0f), EscapeTime(0.0f), PatrolRadius(0.0f),
		DetectRange(0.0f), AttackShortRange(0.0f), AttackLongRange(0.0f),
		LostTargetDistance(0.0f), CaptureCount(0.0f), StunCount(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern5_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern5_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern5_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Pattern9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HeadBreakThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float LegBreakThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float TailCutThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float WingBreakThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float EscapeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackShortRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackLongRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float LostTargetDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CaptureCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float StunCount;
};

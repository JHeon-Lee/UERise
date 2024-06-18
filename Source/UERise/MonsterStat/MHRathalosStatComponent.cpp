// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStat/MHRathalosStatComponent.h"
#include "GameData/MHGameSingleton.h"

// Sets default values for this component's properties
UMHRathalosStatComponent::UMHRathalosStatComponent()
{
	MonsterNum = 1;

	bWantsInitializeComponent = true;
}

void UMHRathalosStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetStat(MonsterNum);
	SetHp(Stat.MaxHp);
	SetHeadBreakRate(Stat.HeadBreakThreshold);
	SetLegBreakRate(Stat.LegBreakThreshold);
	SetTailCutRate(Stat.TailCutThreshold);
	SetWingBreakRate(Stat.WingBreakThreshold);
	SetCaptureCount(Stat.CaptureCount);
	SetStunCount(Stat.StunCount);
}

void UMHRathalosStatComponent::SetStat(float InNewMonsterNum)
{
	MonsterNum = FMath::Clamp(InNewMonsterNum, 1, UMHGameSingleton::Get().MonstersNum);
	SetMonsterStat(UMHGameSingleton::Get().GetMonsterStat(MonsterNum));
	check(Stat.MaxHp > 0.0f);
}

bool UMHRathalosStatComponent::IsHpZero()
{
	return CurrentHp <= 0 ? true : false;
}

bool UMHRathalosStatComponent::IsHp10Percent()
{
	return (CurrentHp > 0 && CurrentHp < Stat.MaxHp * 0.1)  ? true : false;
}

bool UMHRathalosStatComponent::IsHp75Percent()
{
	return (CurrentHp > 0 && CurrentHp < Stat.MaxHp * 0.75) ? true : false;
}

float UMHRathalosStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

float UMHRathalosStatComponent::ApplyHeadBreakRate(float InHeadBreakRate)
{
	const float PrevHeadBreakRate = CurrentHeadBreakRate;
	const float ActualHeadBreakRate = FMath::Clamp<float>(InHeadBreakRate, 0, InHeadBreakRate);

	SetHeadBreakRate(PrevHeadBreakRate - ActualHeadBreakRate);
	if (CurrentHeadBreakRate <= KINDA_SMALL_NUMBER)
	{
		OnHeadBreak.Broadcast();
		SetHeadBreakRate(Stat.HeadBreakThreshold);
	}

	return ActualHeadBreakRate;
}

float UMHRathalosStatComponent::ApplyLegBreakRate(float InLegBreakRate)
{
	const float PrevLegBreakRate = CurrentLegBreakRate;
	const float ActualLegBreakRate = FMath::Clamp<float>(InLegBreakRate, 0, InLegBreakRate);

	SetLegBreakRate(PrevLegBreakRate - ActualLegBreakRate);
	if (CurrentLegBreakRate <= KINDA_SMALL_NUMBER)
	{
		OnLegBreak.Broadcast();
		SetLegBreakRate(Stat.LegBreakThreshold);
	}

	return ActualLegBreakRate;
}

float UMHRathalosStatComponent::ApplyTailCutRate(float InTailCutRate)
{
	const float PrevTailCutRate = CurrentTailCutRate;
	const float ActualTailCutRate = FMath::Clamp<float>(InTailCutRate, 0, InTailCutRate);

	SetTailCutRate(PrevTailCutRate - ActualTailCutRate);
	if (CurrentTailCutRate <= KINDA_SMALL_NUMBER)
	{
		OnTailCut.Broadcast();
	}

	return ActualTailCutRate;
}

float UMHRathalosStatComponent::ApplyWingBreakRate(float InWingBreakRate)
{
	const float PrevWingBreakRate = CurrentWingBreakRate;
	const float ActualWingBreakRate = FMath::Clamp<float>(InWingBreakRate, 0, InWingBreakRate);

	SetWingBreakRate(PrevWingBreakRate - ActualWingBreakRate);
	if (CurrentWingBreakRate <= KINDA_SMALL_NUMBER)
	{
		OnWingBreak.Broadcast();
		SetWingBreakRate(Stat.WingBreakThreshold);
	}

	return ActualWingBreakRate;
}

float UMHRathalosStatComponent::ApplyCaptureCount(float InCaptureCount)
{
	const float PrevCaptureCount = CurrentCaptureCount;
	const float ActualCaptureCount = FMath::Clamp<float>(InCaptureCount, 0, InCaptureCount);

	SetCaptureCount(PrevCaptureCount - ActualCaptureCount);
	if (CurrentCaptureCount <= KINDA_SMALL_NUMBER)
	{
		OnCapture.Broadcast();
	}

	return ActualCaptureCount;
}

float UMHRathalosStatComponent::ApplyStunCount(float InStunCount)
{
	const float PrevStunCount = CurrentStunCount;
	const float ActualStunCount = FMath::Clamp<float>(InStunCount, 0, InStunCount);

	SetStunCount(PrevStunCount - ActualStunCount);
	if (CurrentStunCount <= KINDA_SMALL_NUMBER)
	{
		OnStun.Broadcast();
	}

	return ActualStunCount;
}

void UMHRathalosStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, Stat.MaxHp);
}

void UMHRathalosStatComponent::SetHeadBreakRate(float NewHeadBreakRate)
{
	CurrentHeadBreakRate = FMath::Clamp<float>(NewHeadBreakRate, 0.0f, Stat.HeadBreakThreshold);
}

void UMHRathalosStatComponent::SetLegBreakRate(float NewLegBreakRate)
{
	CurrentLegBreakRate = FMath::Clamp<float>(NewLegBreakRate, 0.0f, Stat.LegBreakThreshold);
}

void UMHRathalosStatComponent::SetTailCutRate(float NewTailCutRate)
{
	CurrentTailCutRate = FMath::Clamp<float>(NewTailCutRate, 0.0f, Stat.TailCutThreshold);
}

void UMHRathalosStatComponent::SetWingBreakRate(float NewWingBreakRate)
{
	CurrentWingBreakRate = FMath::Clamp<float>(NewWingBreakRate, 0.0f, Stat.WingBreakThreshold);
}

void UMHRathalosStatComponent::SetCaptureCount(float NewCaptureCount)
{
	CurrentCaptureCount = FMath::Clamp<float>(NewCaptureCount, 0.0f, Stat.CaptureCount);
}

void UMHRathalosStatComponent::SetStunCount(float NewStunCount)
{
	CurrentStunCount = FMath::Clamp<float>(NewStunCount, 0.0f, Stat.StunCount);
}

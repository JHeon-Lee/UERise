// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MHMonsterStat.h"
#include "MHRathalosStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHeadBreakDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLegBreakDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTailCutDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWingBreakDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCaptureDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStunDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMHRathalosStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMHRathalosStatComponent();
		
protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHeadBreakDelegate OnHeadBreak;
	FOnLegBreakDelegate OnLegBreak;
	FOnTailCutDelegate OnTailCut;
	FOnWingBreakDelegate OnWingBreak;
	FOnCaptureDelegate OnCapture;
	FOnStunDelegate OnStun;

	void SetStat(float InNewMonsterNum);
	FORCEINLINE void SetMonsterStat(const FMHMonsterStat& InStat) { Stat = InStat; }

	FORCEINLINE const FMHMonsterStat& GetStat() const { return Stat; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }

	UFUNCTION(BlueprintCallable)
	bool IsHpZero();

	UFUNCTION(BlueprintCallable)
	bool IsHp10Percent();

	UFUNCTION(BlueprintCallable)
	bool IsHp75Percent();

	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
	float ApplyHeadBreakRate(float InHeadBreakRate);

	UFUNCTION(BlueprintCallable)
	float ApplyLegBreakRate(float InLegBreakRate);

	UFUNCTION(BlueprintCallable)
	float ApplyTailCutRate(float InTailCutRate);

	UFUNCTION(BlueprintCallable)
	float ApplyWingBreakRate(float InWingBreakRate);

	UFUNCTION(BlueprintCallable)
	float ApplyCaptureCount(float InCaptureCount);

	UFUNCTION(BlueprintCallable)
	float ApplyStunCount(float InStunCount);

protected:
	void SetHp(float NewHp);
	void SetHeadBreakRate(float NewHeadBreakRate);
	void SetLegBreakRate(float NewLegBreakRate);
	void SetTailCutRate(float NewTailCutRate);
	void SetWingBreakRate(float NewWingBreakRate);
	void SetCaptureCount(float NewCaptureCount);
	void SetStunCount(float NewStunCount);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHeadBreakRate;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLegBreakRate;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentTailCutRate;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentWingBreakRate;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentCaptureCount;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentStunCount;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MonsterNum;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMHMonsterStat Stat;
};

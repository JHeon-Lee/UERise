// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MHUserWidget.h"
#include "UtusiHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UUtusiHPBarWidget : public UMHUserWidget
{
	GENERATED_BODY()
	
public:
	UUtusiHPBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	



public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHP = NewMaxHp; }

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);


protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHP;
};

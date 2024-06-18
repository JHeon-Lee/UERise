// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckEscapeRoar.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTDecorator_CheckEscapeRoar : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckEscapeRoar();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

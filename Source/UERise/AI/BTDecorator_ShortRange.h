// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ShortRange.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTDecorator_ShortRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ShortRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

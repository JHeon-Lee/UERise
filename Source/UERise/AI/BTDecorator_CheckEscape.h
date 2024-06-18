// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckEscape.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTDecorator_CheckEscape : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckEscape();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

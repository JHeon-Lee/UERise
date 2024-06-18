// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ArrivedEscapePos.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTDecorator_ArrivedEscapePos : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ArrivedEscapePos();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

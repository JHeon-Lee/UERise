// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackPattern8_1.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTTask_AttackPattern8_1 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackPattern8_1();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

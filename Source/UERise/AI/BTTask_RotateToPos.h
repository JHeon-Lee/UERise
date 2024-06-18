// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToPos.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTTask_RotateToPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RotateToPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

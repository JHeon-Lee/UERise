// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UBTService_CheckDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

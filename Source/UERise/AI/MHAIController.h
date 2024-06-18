// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MHAIController.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMHAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMHAIController();

	void RunAI();
	void StopAI();

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};

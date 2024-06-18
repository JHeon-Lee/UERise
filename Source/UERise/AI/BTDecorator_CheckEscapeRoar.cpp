// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckEscapeRoar.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTDecorator_CheckEscapeRoar::UBTDecorator_CheckEscapeRoar()
{
	NodeName = TEXT("CheckEscapeRoar");
}

bool UBTDecorator_CheckEscapeRoar::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	bool HasEscapeRoared = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_HASESCAPEROARED);
	bResult = HasEscapeRoared;

	return bResult;
}

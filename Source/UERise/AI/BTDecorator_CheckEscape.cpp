// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckEscape.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTDecorator_CheckEscape::UBTDecorator_CheckEscape()
{
	NodeName = TEXT("CheckEscape");
}

bool UBTDecorator_CheckEscape::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	bool HaveToEscape = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_HAVETOESCAPE);
	bool HasEscaped = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_HASESCAPED);

	if (AIPawn->Get270secElapsedAI())
	{
		if (!HasEscaped)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HAVETOESCAPE, true);
			bResult = true;
		}
		else
		{
			bResult = false;
		}
	}
	else
	{
		bResult = false;
	}

	return bResult;
}

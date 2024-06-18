// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckAnger.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTDecorator_CheckAnger::UBTDecorator_CheckAnger()
{
	NodeName = TEXT("CheckAnger");
}

bool UBTDecorator_CheckAnger::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	bool IsAnger = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISANGER);
	bool HasAngered = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_HASANGERED);

	if (AIPawn->IsHp75AI())
	{
		if (!IsAnger && !HasAngered)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISANGER, true);
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

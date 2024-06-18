// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackPattern4.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_AttackPattern4::UBTTask_AttackPattern4()
{
}

EBTNodeResult::Type UBTTask_AttackPattern4::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIRathalosAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN4, FVector(2, true, false));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackPattern4AI();
	return EBTNodeResult::InProgress;
}

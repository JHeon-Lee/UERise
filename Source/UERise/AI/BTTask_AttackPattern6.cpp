// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackPattern6.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_AttackPattern6::UBTTask_AttackPattern6()
{
}

EBTNodeResult::Type UBTTask_AttackPattern6::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN6, FVector(4, true, true));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackPattern6AI();
	return EBTNodeResult::InProgress;
}

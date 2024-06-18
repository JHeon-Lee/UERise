// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackPattern5_3.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_AttackPattern5_3::UBTTask_AttackPattern5_3()
{
}

EBTNodeResult::Type UBTTask_AttackPattern5_3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN5_3, FVector(3, true, true));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackPattern8AI();
	return EBTNodeResult::InProgress;
}

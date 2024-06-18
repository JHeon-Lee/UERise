// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EscapeRoar.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_EscapeRoar::UBTTask_EscapeRoar()
{
}

EBTNodeResult::Type UBTTask_EscapeRoar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    FAIRathalosRoarFinished OnRoarFinished;
    OnRoarFinished.BindLambda(
        [&]()
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HASESCAPEROARED, true);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRoarDelegate(OnRoarFinished);

    AIPawn->EscapeRoarAI();
    return EBTNodeResult::InProgress;
}

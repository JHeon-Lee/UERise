// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Roar.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_Roar::UBTTask_Roar()
{
}

EBTNodeResult::Type UBTTask_Roar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HASROARED, true);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRoarDelegate(OnRoarFinished);

    AIPawn->RoarAI();
    return EBTNodeResult::InProgress;
}